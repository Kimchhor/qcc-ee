from qvm import qir
from qcc import qasm
def transpile(qc, allow_array_var=False):
    """
    Transpiles a quantum circuit to QIR (Quantum Intermediate Representation).
    
    Args:
        qc (QuantumCircuit): The quantum circuit to be transpiled.
    
    Returns:
        QIR: The transpiled QIR representation of the quantum circuit.
    """
    return qir.transpile(qc, allow_array_var=allow_array_var)

def qasm(qir):
    return qasm(qir)