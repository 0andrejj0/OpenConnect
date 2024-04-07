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
    args = parser.parse_args()

    configure_command = 'cmake ' + \
                        '-DCMAKE_INSTALL_PREFIX:PATH=' + f'{dir_path}/install ' + \
                        '-S ' + f'{dir_path}/desktop ' + \
                        '-B ' + f'{dir_path}/build ' + \
                        '-D ABSL_PROPAGATE_CXX_STD=ON ' + \
                        '-D CMAKE_EXPORT_COMPILE_COMMANDS=ON ' + \
                        '-D COMMON_DIR=' + f'{dir_path}/common '
    
    assert(os.system(configure_command) == 0)

    build_command = 'make -C' + \
                    f'{dir_path}/build '

    install_command = 'make -C' + \
                    f'{dir_path}/build ' + \
                    'install '

    assert(os.system(build_command) == 0)

    assert(os.system(install_command) == 0)

main()