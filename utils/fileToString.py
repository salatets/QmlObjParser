# convert files with code to c string
import argparse


def convert(in_lines):
    maxLen = 0
    for line in lines:
        if len(line) > maxLen:
            maxLen = len(line)

    out_lines = []
    for line in in_lines:
        out_lines.append(
                '"'+line[0:-1] + (" " * (maxLen - len(line))) + '\\n" \\\n')

    out_lines[-1] = out_lines[-1][0:-3] + ";\n\n"

    return out_lines


def parse_args():
    inputfiles = []
    outputfile = ''

    parser = argparse.ArgumentParser()
    parser.add_argument(
            '-i', '--input', nargs='+',
            help='<Required> input files', required=True)
    parser.add_argument(
            '-o', '--output', nargs=1,
            help='<Required> output file', required=True)

    for key, value in parser.parse_args()._get_kwargs():
        if value is not None:
            if key == 'input':
                inputfiles = value
            elif key == 'output':
                outputfile = value[0]

    return inputfiles, outputfile


# truncate other/some/thing.h to thing
def get_filename(path):
    return path.rsplit('/', maxsplit=1)[-1].split('.', maxsplit=1)[0]


inputfiles, outputfile = parse_args()

with open(outputfile, 'w') as out_file:
    header_name = get_filename(outputfile).upper() + "_H"
    lines = []
    out_file.write("#ifndef " + header_name +
                   "\n#define " + header_name + "\n\n")

    for inputfile in inputfiles:
        with open(inputfile, 'r') as in_file:
            lines = in_file.readlines()

        out_file.write("char " + get_filename(inputfile) +
                       "[] = " + '"\\n" \\\n')
        lines = convert(lines)
        out_file.write(''.join(lines))

    out_file.write("#endif //" + header_name)

print("done")
