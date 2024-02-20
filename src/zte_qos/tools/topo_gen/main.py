import argparse

from spec import *


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='A network topology generator which generates the corresponding NED file.')
    parser.add_argument('-i', '--info-dir', type=str, help='directory holding the info files')
    parser.add_argument('-n', '--net', type=str, help=f'name of the generated network, default = {Network.NAME_DEFAULT}', default=Network.NAME_DEFAULT)
    parser.add_argument('-j', '--json', type=int, help='whether to dump yaml dict to json, default = 0 (1 is True)', default=0)
    FLAGS, unparsed = parser.parse_known_args()

    network = Network(info_dir=FLAGS.info_dir, name=FLAGS.net)
    if FLAGS.json == 1:
        for s in network.switches.values():
            s.to_local_json(path=os.path.join(network.info_dir, f'{s.id}.json'))
