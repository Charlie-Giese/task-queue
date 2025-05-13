import subprocess
import os

BUILD_DIR = "build_benchmark"
BENCHMARKS = [
    "throughput_benchmark",
    "latency_benchmark",
    "scalability_benchmark",
    "overhead_benchmark",
]
OUTPUT_FILE = "benchmark_results.txt"


def run():
    os.makedirs(BUILD_DIR, exist_ok=True)
    print("=== Building benchmarks ===")
    subprocess.run(["cmake", "-B", BUILD_DIR, "-S", ".",
                   "-DCMAKE_BUILD_TYPE=Release"], check=True)
    subprocess.run(["cmake", "--build", BUILD_DIR, "--target"] +
                   BENCHMARKS + ["-j"], check=True)

    with open(OUTPUT_FILE, "w") as out:
        for exe in BENCHMARKS:
            print(f"\n=== Running {exe} ===\n")
            out.write(f"=== {exe} ===\n")
            result = subprocess.run(
                [os.path.join(BUILD_DIR, exe)], capture_output=True, text=True)
            print(result.stdout)
            out.write(result.stdout + "\n")

    print(f"\n✅ All benchmarks completed. Results saved to {OUTPUT_FILE}")


if __name__ == "__main__":
    run()
