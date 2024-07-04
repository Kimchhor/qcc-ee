

class QASMReg<bits<16> Enc, string n> : Register<n> {
  let HWEncoding = Enc;
  let Namespace = "QASM";
}

// General register
class GRi<bits<16> Enc, string n> : QASMReg<Enc, n>;

// General-purpose registers
foreach I = 0-255 in {
  def R#I : GRi<I, "R"#I>;
}

def GPR : RegisterClass<"QASM", [i32], 32,
                            (add (sequence "R%u", 0, 255))>;

foreach I = 0-255 in {
  def F#I : GRi<I, "F"#I>;
}

def FPR : RegisterClass<"QASM", [f32], 32, (add (sequence "F%u", 0, 255))>;

def FPROpnd : RegisterOperand<FPR>;

def GPROpnd : RegisterOperand<GPR>;

class QASMQuantumReg<bits<16> Enc, string n> : Register<n> {
  let HWEncoding = Enc;
  let Namespace = "QASM";
}

class QPRi<bits<16> Enc, string n>: QASMQuantumReg<Enc, n>; 

// Define all 256 quantum-operand registers
foreach I = 0-255 in {
    def QR#I : QPRi<I, "QR"#I>;
}

// v2f64 :  2 x f64 vector value
// first value: is gate ID
// seconde value:
//   - 0: if it is single-qubit gate
//   - CNOT ID: if it is SWAP or CNOT
//   - float: if it is rotate gate
def QPR : RegisterClass<"QASM", [qubit], 32, (add (sequence "QR%u", 0, 255))>;

def QPROpnd : RegisterOperand<QPR>;

// Define IDAddr and UDAddr
