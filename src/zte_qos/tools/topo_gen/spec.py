import os
import json
from pathlib import Path

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

    def to_local_json(self, path: str) -> None:
        """ to_local_json saves yaml_dict to local json file. """
        with open(path, 'w') as f:
            json.dump(self.yaml_dict, f, indent=2)

    def __str__(self):
        return f'Switch[{self.id}]'

    def __repr__(self):
        return self.__str__()


class Network:
    NAME_DEFAULT = 'ZteNet'

    def __init__(self, info_dir: str, name: str = NAME_DEFAULT):
        self.info_dir = info_dir
        self.name = name
        if not os.path.isdir(self.info_dir):
            raise NotADirectoryError(f'{self.info_dir} is not a directory')
        self.switches = self.load_topology()

    def load_topology(self) -> dict[int, Switch]:
        res: dict[int, Switch] = {}
        for info_fn in os.listdir(self.info_dir):
            if not info_fn.endswith('.yaml'):
                continue
            cur_switch = Switch(os.path.join(self.info_dir, info_fn))
            res[cur_switch.id] = cur_switch
        return res

    def __str__(self):
        return f'Network-{self.name}'

    def __repr__(self):
        return self.__str__()
