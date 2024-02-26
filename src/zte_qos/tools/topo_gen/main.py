import argparse

from spec import *


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='A network topology generator which generates the corresponding NED file.')
    parser.add_argument('-i', '--info-dir', type=str, help='directory holding the info files', required=True)
    parser.add_argument('-t', '--trace-fn', type=str, help='path holding the trace file', required=True)
    parser.add_argument('-n', '--net', type=str, help=f'name of the generated network, default = {Network.NAME_DEFAULT}', default=Network.NAME_DEFAULT)
    parser.add_argument('-d', '--description', type=str, help=f'description of the generated network, default = None', default=None)
    parser.add_argument('-ini', '--ini-dir', type=str, help=f'directory holding the omnetpp.ini file, default = {Network.INI_DIR_DEFAULT}', default=Network.INI_DIR_DEFAULT)
    parser.add_argument('-c', '--config-ini', type=str, help=f'GenNets.ini file to add the new Config block for the generated network, default = {Network.CONFIG_INI_DEFAULT}', default=Network.CONFIG_INI_DEFAULT)
    parser.add_argument('-ns', '--namespace', type=str, help=f'namespace of the network, default = {Network.NAMESPACE_DEFAULT}', default=Network.NAMESPACE_DEFAULT)
    parser.add_argument('-o', '--out-dir', type=str, help=f'directory to output the NED file, default = {Network.NED_DIR_DEFAULT}', default=Network.NED_DIR_DEFAULT)
    parser.add_argument('-j', '--json', type=int, help='whether to dump yaml dict to json, default = 0 (1 is True)', default=0)
    parser.add_argument('-w', '--num-workers', type=int, help=f'number of workers/cores to load switches info files, default = {Network.NUM_WORKERS_DEFAULT}', default=Network.NUM_WORKERS_DEFAULT)
    FLAGS, unparsed = parser.parse_known_args()

    network = Network(info_dir=FLAGS.info_dir, trace_fn=FLAGS.trace_fn,
                      name=FLAGS.net, description=FLAGS.description,
                      ini_dir=FLAGS.ini_dir, config_ini=FLAGS.config_ini,
                      out_dir=FLAGS.out_dir, namespace=FLAGS.namespace,
                      use_json=(FLAGS.json == 1), num_workers=FLAGS.num_workers)
    network.generate_ned()
