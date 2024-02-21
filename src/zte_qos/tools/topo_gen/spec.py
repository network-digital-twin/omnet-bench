import os
import json
import itertools
from pathlib import Path
from typing import Tuple

import yaml


class Switch:
    """ Switch model. """
    def __init__(self, info_fn: str):
        self.info_fn = info_fn
        if not self.info_fn.endswith('.yaml'):
            raise ValueError(f'Not a yaml file: {self.info_fn}')
        self.yaml_dict: dict = yaml.load(open(self.info_fn), Loader=yaml.FullLoader)
        # fields
        self.id: int = int(Path(self.info_fn).stem)
        self.port_bw: dict[str, float] = self.yaml_dict['ports']    # ID => bandwidth-bps
        self.qos: str = self.yaml_dict['qos']
        self.routing: dict[int, dict] = self.yaml_dict['routing']
        self.switch_type: str = self.yaml_dict['type']

    def to_ned(self, indent: int = 4) -> list[str]:
        """
        Return a list formatted string lines of the switch NED block.
        :param indent: indent level
        """
        indent_str = ' ' * indent
        return [
            f's_{self.id}: Switch {{',
            indent_str + f'sid = {self.id};',
            f'}}'
        ]

    def to_local_json(self, path: str) -> None:
        """
        Save yaml_dict to local json file.
        :param path: path of the resulting json file
        """
        with open(path, 'w') as f:
            json.dump(self.yaml_dict, f, indent=2)

    def __str__(self):
        return f'Switch[{self.id}]'

    def __repr__(self):
        return self.__str__()


class Link:
    DELAY_DEFAULT: float = 10  # propagation delay - us

    """ Link model. """
    def __init__(self, src: Switch, src_port: str, dst: Switch, dst_port: str = None,
                 pair_id: int = 0, delay: float = DELAY_DEFAULT) -> None:
        self.src = src.id
        self.src_port = src_port
        if src_port not in src.port_bw.keys():
            raise ValueError(f'Source port {src_port} is not in Switch {src.id}')
        self.dst = dst.id
        self.dst_port = dst_port
        if dst_port is not None and dst_port not in dst.port_bw.keys():
            raise ValueError(f'Destination port {dst_port} is not in Switch {dst.id}')
        # may have multiple identical src-dst pairs - need a pair_id to differentiate
        self.pair_id = pair_id
        # minimum bandwidth of two end ports; if dst port not specified, uses src port bandwidth
        self.bw = min(
            src.port_bw[self.src_port],
            dst.port_bw[self.dst_port] if self.dst_port is not None else float('inf')
        )
        self.delay = delay

    def to_ned(self, indent: int = 4) -> list[str]:
        """
        Return a list formatted string lines of the network link NED block.
        :param indent: indent level
        """
        indent_str = ' ' * indent
        return [
            f'l_{self.src}_{self.dst}__{self.pair_id}: Link {{',
            indent_str + f'bitrate = {self.bw}bps;',
            indent_str + f'delay = {self.delay}us;',
            indent_str + f'@metadata(srcPort="{self.src_port}");',
            f'}}'
        ]

    def __str__(self):
        return f'Link[{self.src}~{self.dst}][{self.pair_id}]'

    def __repr__(self):
        return self.__str__()


class Network:
    """ Network model. """
    NAME_DEFAULT = 'ZteNet'
    NED_DIR_DEFAULT = '../../src/networks/gen/'
    INI_DIR_DEFAULT = '../../simulations/'

    def __init__(self, info_dir: str, trace_fn: str,
                 name: str = NAME_DEFAULT, description: str = None, ini_dir: str = INI_DIR_DEFAULT,
                 out_dir: str = NED_DIR_DEFAULT, use_json: bool = False):
        self.info_dir = info_dir
        if not os.path.isdir(self.info_dir):
            raise NotADirectoryError(f'{self.info_dir} is not a directory')
        self.trace_fn = trace_fn
        if not os.path.isfile(self.trace_fn):
            raise FileNotFoundError(f'{self.trace_fn} is not a valid file')
        self.name = name
        self.description = f'{self.name}.' if description is None else description
        self.ini_dir = ini_dir
        if not os.path.isdir(self.ini_dir):
            raise NotADirectoryError(f'{self.ini_dir} is not a directory')
        self.out_dir = out_dir
        if not os.path.isdir(self.out_dir):
            os.makedirs(out_dir)
        self.use_json = use_json
        self.switches, self.links = self.load_topology()

    def load_topology(self) -> Tuple[dict[int, Switch], set[Link]]:
        """ Load topology as a dict of switch_id => switch from the info directory, and a set of links. """
        switches: dict[int, Switch] = {}
        for info_fn in os.listdir(self.info_dir):
            if not info_fn.endswith('.yaml'):
                continue
            cur_switch = Switch(os.path.join(self.info_dir, info_fn))
            switches[cur_switch.id] = cur_switch
        links: set[Link] = set()
        link_map: dict[int, dict[int, set[str]]] = {
            src: {
                dst: set() for dst in switches.keys()
            } for src in switches.keys()
        }
        for s in switches.values():
            src = s.id
            for route in s.routing.values():
                dst, src_port = route['next_hop'], route['port']
                if src_port not in link_map[src][dst]:
                    links.add(Link(src=switches[src], src_port=src_port,
                                   dst=switches[dst], dst_port=None,
                                   pair_id=len(link_map[src][dst])))
                    link_map[src][dst].add(src_port)
        return switches, links

    def generate_ned(self, indent: int = 4) -> None:
        """
        Generate NED file representing the network.
        :param indent: indent level
        """
        # export .json files if use_json
        if self.use_json:
            for s in self.switches.values():
                s.to_local_json(path=os.path.join(self.info_dir, f'{s.id}.json'))
        # generate topology
        out_fn = os.path.join(self.out_dir, f'{self.name}.ned')
        with open(out_fn, 'w') as f:
            f.write('//\n'
                    '// Auto-generated by "tools/topo_gen/main.py". DO NOT EDIT.\n'
                    '//\n')
            f.write('\n')
            # package
            f.write('package zte_qos.networks.gen;\n')
            f.write('\n')
            # import
            f.write('import zte_qos.terminal.Terminal;\n'
                    'import zte_qos.switch.Switch;\n'
                    'import zte_qos.link.Link;\n')
            f.write('\n')
            # network
            f.write('\n'.join(self.to_network_ned(indent)))
            f.write('\n')
        # TODO: logging and suggest in stdout to put Config into GenNets.ini

    def to_network_ned(self, indent: int = 4) -> list[str]:
        """
        Return a list formatted string lines of the network NED block.
        :param indent: indent level
        """
        indent_str = ' ' * indent
        return [
            f'// {self.description}',
            f'network {self.name}',
            f'{{',
            # parameters
            *[(indent_str + line) for line in self.to_parameters_ned(indent)],
            # submodules
            *[(indent_str + line) for line in self.to_submodules_ned(indent)],
            f'}}'
        ]

    def to_parameters_ned(self, indent: int = 4) -> list[str]:
        """
        Return a list formatted string lines of the network parameters NED block.
        :param indent: indent level
        """
        indent_str = ' ' * indent
        # TODO: is there a cleaner way?
        traceFileRel = os.path.relpath(self.trace_fn, self.ini_dir).replace('\\', '/')
        infoDirRel = os.path.relpath(self.info_dir, self.ini_dir).replace('\\', '/')
        if not infoDirRel.endswith('/'):
            infoDirRel += '/'
        infoFileType: str = 'json' if self.use_json else 'yaml'
        return [
            f'parameters:',
            indent_str + f't.traceFile = "{traceFileRel}";',
            indent_str + f's_*.infoDir = "{infoDirRel}";',
            indent_str + f's_*.infoFileType = "{infoFileType}";'
        ]

    def to_submodules_ned(self, indent: int = 4) -> list[str]:
        """
        Return a list formatted string lines of the network submodules NED block.
        :param indent: indent level
        """
        indent_str = ' ' * indent
        return [
            f'submodules:',
            # terminal
            indent_str + f'// terminal',
            indent_str + f't: Terminal;',
            # switches
            indent_str + f'// switches',
            *[(indent_str + line) for line in itertools.chain(*[*[s.to_ned(indent) for s in self.switches.values()]])],
            # links
            indent_str + f'// links',
            *[(indent_str + line) for line in itertools.chain(*[*[link.to_ned(indent) for link in self.links]])],
        ]

    def __str__(self):
        return f'Network-{self.name}'

    def __repr__(self):
        return self.__str__()
