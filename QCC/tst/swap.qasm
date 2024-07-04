OPENQASM 2.0;
include "qelib1.inc";
qreg q[2];
creg c[0];
swap q[0], q[1];
h q[1];
swap q[0], q[1];
h q[1];
