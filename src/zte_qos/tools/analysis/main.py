import json
import os
import argparse
import math
from pathlib import Path
from typing import Tuple
from datetime import datetime
import itertools

import pandas as pd


PacketRes = dict[int, dict[int, list[dict]]]


class Analyzer:
    """ Result Analyzer generates corresponding res-simulation.json, res-packets.csv, and res-flows.csv files. """
    def __init__(self, log_fn: str):
        self.log_fn = log_fn
        if not self.log_fn.endswith('.log') or not os.path.isfile(self.log_fn):
            raise ValueError(f'Invalid log filename: "{self.log_fn}"')
        self.log_dir = Path(self.log_fn).parent
        self.log_fn_prefix = Path(self.log_fn).stem
        self.sim_res, self.pkt_res = self.load_res_from_log()

    def load_res_from_log(self) -> Tuple[dict, PacketRes]:
        """ Load simulation and packet results from log files. """
        sim_res = {}
        pkt_res: PacketRes = {}
        num_events = -1
        with open(self.log_fn, 'r') as f:
            while True:
                line: str = f.readline()
                if not line:
                    break
                if line.startswith('<!>') and 'event' in line:
                    # retrieve number of events
                    num_events = int(line.strip().rsplit('event', maxsplit=1)[-1].strip()[1:])
                    continue
                if not line.startswith('[RES]'):
                    continue
                log_json: dict = json.loads(line.strip().split(' ', maxsplit=1)[-1])
                if log_json['type'] == 'simulation':
                    sim_res = log_json
                elif log_json['type'] == 'packet':
                    src_id = log_json['metrics']['src']
                    dst_id = log_json['metrics']['dst']
                    if src_id not in pkt_res:
                        pkt_res[src_id] = {}
                    if dst_id not in pkt_res[src_id]:
                        pkt_res[src_id][dst_id] = []
                    pkt_res[src_id][dst_id].append(log_json)
        sim_res['num_events'] = num_events
        return sim_res, pkt_res

    def get_flat_pkt_res(self) -> list[dict]:
        """ Get all packet results from as a flatten list. """
        return list(itertools.chain(*itertools.chain(*[src_dict.values() for src_dict in self.pkt_res.values()])))

    @staticmethod
    def process_pkt_stats(pkt_res: list[dict], detailed_delay_metrics: bool = True) -> dict:
        """
        Process packet statistics given a flat list of packet results.
        :param pkt_res: a flat list of packet results
        :param detailed_delay_metrics: whether to calculate all delay metrics (min, max, avg); if not, calculate avg only
        """
        pkt_stats = {
            'num_packets': len(pkt_res)
        }
        # calculate packet delay
        delays = [(res['metrics']['end_ts'] - res['metrics']['start_ts']) for res in pkt_res if res['metrics']['drop'] == 0]
        avg_delay = sum(delays) / len(delays) if len(delays) > 0 else float('nan')
        pkt_stats['delay'] = {
            'min': min(delays) if len(delays) > 0 else float('nan'),
            'max': max(delays) if len(delays) > 0 else float('nan'),
            'avg': avg_delay
        } if detailed_delay_metrics else avg_delay
        # calculate packet jitter
        pkt_stats['jitter'] = math.sqrt(sum([
            math.pow(d - avg_delay, 2) for d in delays
        ]) / len(delays)) if len(delays) > 0 else float('nan')
        # get number of packet drops
        pkt_stats['drop_rate'] = sum([
            res['metrics']['drop'] for res in pkt_res
        ]) / len(pkt_res) if len(pkt_res) > 0 else float('nan')
        return pkt_stats

    def gen_sim_res(self) -> None:
        """ Calculate and generate simulation results to .json file. """
        sim_res_fn = os.path.join(self.log_dir, f'{self.log_fn_prefix}-res-simulation.json')
        start_epoch = int(self.sim_res['metrics']['start'])
        end_epoch = int(self.sim_res['metrics']['end'])
        sim_res = {
            'start': str(datetime.fromtimestamp(start_epoch / 1e9)),
            'end': str(datetime.fromtimestamp(end_epoch / 1e9)),
            'elapsed': (end_epoch - start_epoch) / 1e9,
            'num_events': self.sim_res['num_events'],
            **self.process_pkt_stats(pkt_res=self.get_flat_pkt_res(), detailed_delay_metrics=True)
        }
        sim_res['eps'] = sim_res['num_events'] / sim_res['elapsed'] if sim_res['elapsed'] != 0 else float('nan')
        print(json.dumps(sim_res, indent=2))
        with open(sim_res_fn, 'w') as f:
            json.dump(sim_res, f, indent=2)
        print(f'Successfully generated "{sim_res_fn}"')

    def gen_pkt_res(self) -> None:
        """ Dump packet-wise results to .csv file. """
        pkt_res_fn = os.path.join(self.log_dir, f'{self.log_fn_prefix}-res-packets.csv')
        pkt_rec: list[dict] = []
        for res in self.get_flat_pkt_res():
            pkt_rec.append({
                **res['metrics'],
                'module': res['module']
            })
        pkt_df = pd.DataFrame.from_records(
            pkt_rec,
            columns=['pid', 'src', 'dst', 'start_ts', 'end_ts', 'drop', 'module']
        )
        pkt_df.to_csv(pkt_res_fn, index=False)
        print(f'Successfully generated "{pkt_res_fn}"')

    def gen_flow_res(self) -> None:
        """ Dump flow-wise results to .csv file. """
        flow_res_fn = os.path.join(self.log_dir, f'{self.log_fn_prefix}-res-flows.csv')
        flow_rec: list[dict] = []
        for src, dst_dict in sorted(self.pkt_res.items()):
            for dst, res_list in sorted(dst_dict.items()):
                flow_rec.append({
                    'src': src,
                    'dst': dst,
                    **self.process_pkt_stats(pkt_res=res_list, detailed_delay_metrics=False)
                })
        flow_df = pd.DataFrame.from_records(
            flow_rec,
            columns=['src', 'dst', 'num_packets', 'delay', 'jitter', 'drop_rate']
        )
        flow_df.to_csv(flow_res_fn, index=False, header=False)
        print(f'Successfully generated "{flow_res_fn}"')


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='A result analyzer of the OMNeT++ benchmark.')
    parser.add_argument('-l', '--log', type=str, help='input log file', required=True)
    FLAGS, unparsed = parser.parse_known_args()
    # generate
    ana = Analyzer(log_fn=FLAGS.log)
    ana.gen_sim_res()
    ana.gen_pkt_res()
    ana.gen_flow_res()
