
from qcc import qasm

if __name__ == "__main__":

    try:
        # Run the LLVM project
        qasm_output = qasm("qcl_code.ll")
    except Exception as e:
        print(f"Failed to process quantum code to QASM: {e}")