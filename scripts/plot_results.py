import numpy as np
import matplotlib.pyplot as plt
import argparse
import os


def load_data(data_dir, rates):
    """Loads BER data for specified rates."""
    all_data = {}
    for rate in rates:
        ber_file = os.path.join(data_dir, f"pucch_rate_{rate}_ber.bin")
        try:
            with open(ber_file, "rb") as f:
                ber_values = np.fromfile(f, dtype=np.float64)
            all_data[rate] = ber_values
        except FileNotFoundError:
            print(f"Data file not found for rate {rate}. Skipping.")
            all_data[rate] = None
    return all_data


def plot_combined_results(data_dir, rates):
    """Plots combined BER results for multiple rates on a single graph."""

    all_ber_data = load_data(data_dir, rates)

    max_len = 0
    for ber_data in all_ber_data.values():
        if ber_data is not None:
            max_len = max(max_len, len(ber_data))

    snr_values = np.linspace(-5, 10, max_len)

    plt.figure(figsize=(12, 8))

    for rate, ber_values in all_ber_data.items():
        if ber_values is not None:

            ber_values = ber_values[: len(snr_values)]

            plt.semilogy(
                snr_values, ber_values, marker="o", linestyle="-", label=f"Rate {rate}"
            )

    plt.xlabel("SNR (dB)")
    plt.ylabel("Bit Error Rate (BER)")
    plt.title("PUCCH Format 2 Decoder - BER vs. SNR")
    plt.grid(True)
    plt.legend()
    plt.tight_layout()
    plt.savefig(os.path.join(data_dir, "ber_plot.png"))
    plt.show()


def main():
    parser = argparse.ArgumentParser(
        description="Plot PUCCH decoder simulation results."
    )
    parser.add_argument(
        "-d",
        "--data_dir",
        type=str,
        default="data",
        help="Directory where the result files are stored.",
    )
    parser.add_argument(
        "-r",
        "--rates",
        type=int,
        nargs="+",
        help="List of rates to plot. If not provided, plots all available rates.",
        default=None,
    )

    args = parser.parse_args()

    if not os.path.isdir(args.data_dir):
        print(f"Error: Data directory '{args.data_dir}' not found.")
        return

    if args.rates is None:

        available_rates = []
        for rate in [2, 4, 6, 8, 11, 13]:
            if os.path.exists(
                os.path.join(args.data_dir, f"pucch_rate_{rate}_ber.bin")
            ):
                available_rates.append(rate)
        rates_to_plot = available_rates
    else:
        rates_to_plot = args.rates

    plot_combined_results(args.data_dir, rates_to_plot)


if __name__ == "__main__":
    main()
