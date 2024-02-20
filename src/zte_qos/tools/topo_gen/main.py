import argparse

from spec import *


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='A network topology generator which generates the corresponding NED file.')
    parser.add_argument('-i', '--info-dir', type=str, help='directory holding the info files')
    parser.add_argument('-t', '--trace-fn', type=str, help='path holding the trace file')
    parser.add_argument('-n', '--net', type=str, help=f'name of the generated network, default = {Network.NAME_DEFAULT}', default=Network.NAME_DEFAULT)
    parser.add_argument('-d', '--description', type=str, help=f'description of the generated network, default = None', default=None)
    parser.add_argument('-ini', '--ini-dir', type=str, help=f'directory holding the omnetpp.ini file, default = {Network.INI_DIR_DEFAULT}', default=Network.INI_DIR_DEFAULT)
    parser.add_argument('-o', '--out-dir', type=str, help=f'directory to output the NED file, default = {Network.NED_DIR_DEFAULT}', default=Network.NED_DIR_DEFAULT)
    parser.add_argument('-j', '--json', type=int, help='whether to dump yaml dict to json, default = 0 (1 is True)', default=0)
    FLAGS, unparsed = parser.parse_known_args()

    network = Network(info_dir=FLAGS.info_dir, trace_fn=FLAGS.trace_fn,
                      name=FLAGS.net, description=FLAGS.description,
                      ini_dir=FLAGS.ini_dir, out_dir=FLAGS.out_dir)
    network.generate_ned()
    # TODO: move this as a function into Network
    if FLAGS.json == 1:
        for s in network.switches.values():
            s.to_local_json(path=os.path.join(network.info_dir, f'{s.id}.json'))
