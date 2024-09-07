"""
Plot data
"""

import click
import os
import statistics
from matplotlib import pyplot as plt
import utils


@click.group()
def cli():
    pass


@cli.command()
@click.argument("result_dir")
@click.argument("model_type")
def plot_magnetization(result_dir, model_type):
    """ plot the magnetization data """

    independent_runs = 3

    lx = 50
    ly = 50
    lz = 50

    tstar_i = 1.000
    tstar_f = 3.000
    tstar_d = 0.010

    mcsteps = 100000
    eqsteps = 10000
    sample_freq = 1000

    # initial_spin = 1
    exchange_energy = 1.00

    lattice_type = "2Dscc"
    nsites = utils.get_total_sites(lattice_type, lx, ly, lz)

    index = 1
    tstar = tstar_i
    results_list = []
    while tstar <= tstar_f:
        tstar_str = "{:.3f}".format(tstar)
        j_str = "{:.3f}".format(exchange_energy)
        i_dir = "{md}_{l}_tstar{t}_J{j}_{lx}x{ly}x{lz}_{m}_{e}_{s}".format(
            md=model_type,
            l=lattice_type,
            t=tstar_str,
            j=j_str,
            lx=lx,
            ly=ly,
            lz=lz,
            m=mcsteps,
            e=eqsteps,
            s=sample_freq,
        )
        run_en = []
        run_mag = []
        for run in range(1, independent_runs + 1):
            run_dir = os.path.join(result_dir, i_dir, "run_{}".format(run))
            results_file = os.path.join(run_dir, "{}_results.csv".format(model_type))
            results_data = utils.read_csv(results_file)
            # mc_step = [x for x, _, _ in results_data]
            en_per_site = [x / nsites for _, x, _ in results_data]
            mag_per_site = [x / nsites for _, _, x in results_data]

            avg_run_en_per_site = statistics.mean(en_per_site)
            avg_run_mag_per_site = statistics.mean(mag_per_site)
            run_en.append(avg_run_en_per_site)
            run_mag.append(avg_run_mag_per_site)

        # get avg EN and MAG from individual runs
        avg_en = statistics.mean(run_en)
        avg_mag = statistics.mean(run_mag)
        results_list.append({"tstar": tstar, "avg_en": avg_en, "avg_mag": avg_mag})

        tstar = tstar_i + index * tstar_d
        index = index + 1

    # [print(x) for x in results_list]
    tstar_as_list = [entry["tstar"] for entry in results_list]
    _ = [entry["avg_en"] for entry in results_list]
    mag_as_list = [entry["avg_mag"] for entry in results_list]

    # plt.plot(tstar_as_list, en_as_list)
    plt.plot(tstar_as_list, mag_as_list)
    plt.show()


if __name__ == "__main__":
    cli()
