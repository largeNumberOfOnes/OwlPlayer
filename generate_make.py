#!/bin/python3

import subprocess as sp



compiler = 'clang++-14'
flags = '-g -std=c++20'
gen_makefile_name = 'Makefile'
output_file_name = 'execs/output.out'
message = '# THIS FILE IS AUTOGEN!!!'

def convert_to_obj(file: str) -> str:
    file = file.replace('had/', '')
    return f'objects/{file[:-4]}.o'
def convert_to_src(file: str) -> str:
    return f'src/{file}'

def add_target_run(file) -> None:
    file.write(f'run:\n\talacritty -e {output_file_name}')
    file.write("\n\n")
def add_target_clear(file) -> None:
    file.write(f'clear:\n\tcd objects; rm *.o')
    file.write("\n\n")
def add_target_car(file) -> None:
    file.write('car: comp run')
    file.write("\n\n")

def main() -> None:
    try:
        includes = sp.check_output('make -s -C src/had get_include_list',
                                                    shell=True, text=True)
        includes = includes[:-1]
        libs = sp.check_output('make -s -C src/had get_libs_list',
                                                    shell=True, text=True)
        libs = libs[:-1]
        had_files = sp.check_output('make -s -C src/had get_source_files',
                                                    shell=True, text=True)
        src_files = sp.check_output('ls src | grep cpp | grep -v test',
                                                    shell=True, text=True)
        files = (src_files + had_files).split()
        targets = []
        for file in files:
            dep = sp.check_output(f'{compiler} -MM src/{file} {includes}',
                                                    shell=True, text=True)
            command = f'\t{compiler} {flags} -c src/{file} ' + \
                                f'{includes} -o {convert_to_obj(file)}\n'
            targets.append('objects/' + dep + command)

        with open(gen_makefile_name, 'w') as file:
            file.write(message)
            file.write("\n\n")

            file.write('\n'.join(targets))

            obj_files = list(map(convert_to_obj, files))
            obj_files_str = ' '.join(obj_files)
            command = f'{compiler} {flags} {obj_files_str} ' + \
                                            f'{libs} -o {output_file_name}'
            file.write('\n\ncomp: ' + obj_files_str + '\n\t' + command)
            file.write("\n\n")

            add_target_run(file)
            add_target_clear(file)
            add_target_car(file)

    except Exception as e:
        print(e)

if __name__ == "__main__":
    main()
