"""
Utils for lattice models
"""
import csv


def read_csv(csv_file):
    """
    Read data from csv

    Args:
        csv_file: input CSV file

    Returns:
        list of lists
    """
    with open(csv_file) as csv_handle:
        reader_obj = csv.reader(csv_handle)

        csv_data = []
        for row in reader_obj:
            row_as_num = []
            for elem in row:
                try:
                    row_as_num.append(float(elem))
                except TypeError:
                    row_as_num.append(elem)

            csv_data.append(row_as_num)

    return csv_data


def get_total_sites(lattice_type, lx, ly, lz):
    """ get total sites for a lattice """

    if lattice_type == "2Dscc":
        nsites = lx * ly
    elif lattice_type == "3Dscc":
        nsites = lx * ly * lz
    else:
        raise ValueError("Invalid lattice type: {}".format(lattice_type))

    return nsites
