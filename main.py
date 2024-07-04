from qiskit import QuantumCircuit
from qvm import qir

qc = QuantumCircuit(3)

qc.h(0)
qc.y(0)
# qc.cx(0, 1)
qc.ccx(0, 1, 2)
# qc.rx(3.14, 0)

measure = qc.measure_all()

# QCC Frontend Transpiler
qir_string = qir.transpile(qc, allow_array_var=True)

with open('qcl.ll', 'w') as f:
    f.write(str(qir_string))
