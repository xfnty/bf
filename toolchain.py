#!/usr/bin/env python3

import os
import sys
import shutil
import subprocess as sp
import multiprocessing as mp
import argparse
import platform
import enum
from dataclasses import dataclass
from typing import List


PROJECT_NAME    = 'bf'

PROJECT_DIR     = os.path.realpath(os.path.dirname(__file__))
ASSETS_DIR      = os.path.join(PROJECT_DIR, 'assets')
CMAKE_DIR       = os.path.join(PROJECT_DIR, '.cmake')
BUILD_DIR       = os.path.join(PROJECT_DIR, 'build')
CMAKE           = shutil.which('cmake')


@dataclass
class BuildTargetDescription:
    exe: str
    folder: str
    cmake_flags: List[str]

class BuildTarget(enum.Enum):
    Debug   = BuildTargetDescription(PROJECT_NAME, 'debug', ['-DCMAKE_BUILD_TYPE=Debug', '-DCMAKE_EXPORT_COMPILE_COMMANDS=1', '-DCMAKE_C_FLAGS=-pg', '-DCMAKE_EXE_LINKER_FLAGS=-pg', '-DCMAKE_SHARED_LINKER_FLAGS=-pg'])
    Release = BuildTargetDescription(PROJECT_NAME, 'release', ['-DCMAKE_BUILD_TYPE=Release'])


def get_cmake_dir_for(target: BuildTarget):
    return os.path.join(CMAKE_DIR, target.value.folder)

def get_build_dir_for(target: BuildTarget):
        return os.path.join(BUILD_DIR, target.value.folder)

def get_exe_dir_for(target: BuildTarget):
    return os.path.join(BUILD_DIR, target.value.folder)

def get_exe_file_for(target: BuildTarget):
    return os.path.join(get_exe_dir_for(target), target.value.exe)


def run_command(
        cmd: list=[],
        cwd: str=PROJECT_DIR,
        status=True,
        output=True,
        exit_on_failure: bool=True):
    if status:
        print(f'> {" ".join(cmd)}', flush=True)

    execution = sp.run(
        cmd,
        cwd=cwd,
        stdout=None if output else sp.PIPE,
        stderr=None if output else sp.PIPE,
    )

    if execution.returncode != 0:
        if not output:
            print(execution.stdout.decode())
            print(execution.stderr.decode())
        if exit_on_failure:
            quit(execution.returncode)
        return execution.returncode

    return execution.returncode


def print_stage_title(s: str):
    print(f'----- {s} -----')


def copy_assets(target: BuildTarget):
    if os.path.exists(ASSETS_DIR):
        shutil.copytree(
            ASSETS_DIR,
            os.path.join(get_exe_dir_for(target), 'assets'),
            dirs_exist_ok=True
        )


def clean():
    print_stage_title('Cleaning up')
    run_command(['git', 'clean', '-Xdf'], status=False)


def configure(target: BuildTarget):
    if not CMAKE:
        print('Could not find CMake executable.')
        quit(-1)

    print_stage_title('Configuring')
    run_command(
        [
            CMAKE,
            '-B', get_cmake_dir_for(target),
            '-S', PROJECT_DIR,
        ]
        + target.value.cmake_flags,
        status=False,
    )

    compile_commands_original_path = os.path.join(get_cmake_dir_for(target), 'compile_commands.json')
    compile_commands_output_path = os.path.join(PROJECT_DIR, 'compile_commands.json')
    if os.path.exists(compile_commands_original_path):
        shutil.copy(compile_commands_original_path, compile_commands_output_path)


def build(target: BuildTarget):
    if not CMAKE:
        print('Could not find CMake executable.')
        quit(-1)

    print_stage_title('Building')
    run_command(
        [
            CMAKE,
            '--build', get_cmake_dir_for(target),
            f'-j{mp.cpu_count()}'
        ],
        status=False,
    )


def run(target: BuildTarget, args: list=[]):
    print_stage_title('Running')
    copy_assets(target)
    exit_code = run_command(
        [get_exe_file_for(target)] + args,
        cwd=get_exe_dir_for(target),
        status=False,
        exit_on_failure=False,
    )
    print_stage_title(f'Exit code {exit_code}')


def main():
    arg_parser = argparse.ArgumentParser(
        description='Configure, build and run this project using CMake.' +
            ' This script is here to replace ".sh" and ".bat" scripts.'
    )
    arg_parser.add_argument(
        '-c', '--configure',
        dest='configure',
        action='store_true',
        help='Configure CMake.'
    )
    arg_parser.add_argument(
        '-b', '--build',
        dest='build',
        action='store_true',
        help='Build the project.'
    )

    run_group = arg_parser.add_mutually_exclusive_group()
    run_group.add_argument(
        '-r', '--run',
        dest='run',
        action='store_true',
        help='Run the compiled executable.'
    )

    arg_parser.add_argument(
        '--clean',
        dest='clean',
        action='store_true',
        help='Delete CMake and build directories.'
    )
    arg_parser.add_argument(
        '--target',
        dest='target',
        default=BuildTarget.Debug.name,
        choices=[t.name for t in list(BuildTarget)],
        help='Specify build target. Available types are ' + ', '.join(f'"{t.name}"' for t in list(BuildTarget)) + '.'
    )
    arg_parser.add_argument(
        '-a', '--args',
        nargs=argparse.REMAINDER,
        default=[],
        dest='args',
        help='Specify command line arguments for the built project to be run with.'
    )

    args = arg_parser.parse_args()
    target = BuildTarget[args.target]

    if args.clean:
        clean()

    if args.configure:
        configure(target)

    if args.build:
        build(target)

    if args.run:
        run(target, args.args)


if __name__ == '__main__':
    try:
        main()
    except KeyboardInterrupt:
        print()
