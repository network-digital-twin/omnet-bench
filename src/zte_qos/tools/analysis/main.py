import json
import os
import argparse
import math
from collections import OrderedDict
from pathlib import Path
from typing import Tuple
from datetime import datetime

import pandas as pd


class Analyzer:
    """ Result Analyzer generates corresponding packets.csv and res-simulation.json files. """
    def __init__(self, log_fn: str):
        self.log_fn = log_fn
        if not self.log_fn.endswith('.log') or not os.path.isfile(self.log_fn):
            raise ValueError(f'Invalid log filename: "{self.log_fn}"')
        self.log_dir = Path(self.log_fn).parent
        self.log_fn_prefix = Path(self.log_fn).stem
        self.sim_res, self.pkt_res = self.load_res_from_log()

    # TODO: pkt_res: OrderedDict[int, dict] --> dict[int, dict[int, dict]], meaning src_id => (dst_id, list[res_dict])
    def load_res_from_log(self) -> Tuple[dict, OrderedDict[int, dict]]:
        """ Load simulation and packet results from log files. """
        sim_res = {}
        pkt_res = OrderedDict()
        with open(self.log_fn, 'r') as f:
            while True:
                line: str = f.readline()
                if not line:
                    break
                if not line.startswith('[RES]'):
                    continue
                log_json: dict = json.loads(line.strip().split(' ', maxsplit=1)[-1])
                if log_json['type'] == 'simulation':
                    sim_res = log_json
                elif log_json['type'] == 'packet':
                    pkt_res[log_json['metrics']['pid']] = log_json
        return sim_res, pkt_res

    def gen_sim_res(self) -> None:
        """ Calculate and generate simulation results to .json file. """
        sim_res_fn = os.path.join(self.log_dir, f'{self.log_fn_prefix}-res-simulation.json')
        start_epoch = int(self.sim_res['metrics']['start'])
        end_epoch = int(self.sim_res['metrics']['end'])
        sim_res = {
            'start': str(datetime.fromtimestamp(start_epoch / 1e9)),
            'end': str(datetime.fromtimestamp(end_epoch / 1e9)),
            'elapsed': (end_epoch - start_epoch) / 1e9
        }
        # calculate packet delay
        delays = [(res['metrics']['end_ts'] - res['metrics']['start_ts']) for res in self.pkt_res.values() if res['metrics']['drop'] == 0]
        sim_res['delay'] = {
            'min': min(delays) if len(delays) > 0 else float('nan'),
            'max': max(delays) if len(delays) > 0 else float('nan'),
            'avg': sum(delays) / len(delays) if len(delays) > 0 else float('nan')
        }
        # calculate packet jitter
        sim_res['jitter'] = math.sqrt(sum([math.pow(d - sim_res['delay']['avg'], 2) for d in delays]) / len(delays)) if len(self.pkt_res) > 0 else float('nan')
        # get number of packet drops
        sim_res['drop'] = sum([res['metrics']['drop'] for res in self.pkt_res.values()])
        print(json.dumps(sim_res, indent=2))
        with open(sim_res_fn, 'w') as f:
            json.dump(sim_res, f, indent=2)
        print(f'Successfully generated "{sim_res_fn}"')

    def gen_pkt_res(self) -> None:
        """ Dump packet-wise results to .csv file. """
        # TODO: process result per src-dst pair
        pkt_res_fn = os.path.join(self.log_dir, f'{self.log_fn_prefix}-res-packets.csv')
        pkt_rec: list[dict] = []
        # TODO: sorted src_id keys and then sorted dst_id keys
        for res in self.pkt_res.values():
            pkt_rec.append({
                **res['metrics'],
                'module': res['module']
            })
        pkt_df = pd.DataFrame.from_records(
            pkt_rec,
            columns=['pid', 'start_ts', 'end_ts', 'drop', 'module']
        )
        pkt_df.to_csv(pkt_res_fn, index=False)
        print(f'Successfully generated "{pkt_res_fn}"')


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='A result analyzer of the OMNeT++ benchmark.')
    parser.add_argument('-l', '--log', type=str, help='input log file', required=True)
    FLAGS, unparsed = parser.parse_known_args()
    # generate
    ana = Analyzer(log_fn=FLAGS.log)
    ana.gen_sim_res()
    ana.gen_pkt_res()
