from qiskit import QuantumCircuit
from qcc_frontend import transpile

qc = QuantumCircuit(2)

qc.h(0)
qc.cx(0, 1)

measure = qc.measure_all()

qir_string = transpile(qc, allow_array_var=True)

with open('qcl_code.ll', 'w') as f:
    f.write(str(qir_string))
    print("file",f)

