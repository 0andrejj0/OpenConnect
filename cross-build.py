#! /usr/bin/python3
import sys
import os
import argparse

def main():
    dir_path = os.path.dirname(os.path.realpath(__file__))
    try:
        os.mkdir(f'{dir_path}/build')
    except:
        pass

    try:
        os.mkdir(f'{dir_path}/install')
    except:
        pass

    parser = argparse.ArgumentParser(description='kek')
    parser.add_argument('--pc_arch', type=str, help='PC arch')
    parser.add_argument('--pc_os', type=str, help='PC OS')
    args = parser.parse_args()

    pc_os = args.pc_os
    assert pc_os in ['linux']

    pc_arch = args.pc_arch
    assert pc_arch in ['x86_64']

    configure_command = 'cmake ' + \
                        '-DCMAKE_INSTALL_PREFIX:PATH=' + f'{dir_path}/install ' + \
                        '-S ' + f'{dir_path}/desktop ' + \
                        '-B ' + f'{dir_path}/build ' + \
                        '-D ABSL_PROPAGATE_CXX_STD=ON ' + \
                        '-D CMAKE_EXPORT_COMPILE_COMMANDS=ON'
    
    os.system(configure_command)

    build_command = 'make -C' + \
                    f'{dir_path}/build '

    install_command = 'make -C' + \
                    f'{dir_path}/build ' + \
                    'install '

    os.system(build_command)

    os.system(install_command)

main()