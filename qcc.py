import subprocess
import os

def qasm(input_file: str):
    build_dir = "./build"
    build_bin: str = os.path.join(build_dir, "bin", "llc")
    march = "qasm"
    target_device = "zx"
    debug="1"

    # Ensure the paths are correct
    if not os.path.isfile(build_bin):
        raise FileNotFoundError(f"LLVM binary not found at {build_bin}")
    if not os.path.isfile(input_file):
        raise FileNotFoundError(f"Input file not found at {input_file}")
   

    # Construct the command
    cmd = [
        build_bin,
        f"-march={march}",
        f"--target-device={target_device}",
        input_file,
        f"--debug={debug}"
    ]

    # Run the command
    result = subprocess.run(cmd)

    if result.returncode != 0:
        print(f"Error running command: {result.stderr}")
        raise RuntimeError(f"Command failed with exit code {result.returncode}")

    return result.stdout
