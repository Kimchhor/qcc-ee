

class QISAReg<bits<16> Enc, string n> : Register<n> {
  let HWEncoding = Enc;
  let Namespace = "QISA";
}

// General register
class GRi<bits<16> Enc, string n> : QISAReg<Enc, n>;

// General-purpose registers
foreach I = 0-255 in {
  def R#I : GRi<I, "R"#I>;
}

def GPR : RegisterClass<"QISA", [i32], 32,
                            (add (sequence "R%u", 0, 255))>;

def GPROpnd : RegisterOperand<GPR>;

class QISAQuantumReg<bits<16> Enc, string n> : Register<n> {
  let HWEncoding = Enc;
  let Namespace = "QISA";
}

class QPRi<bits<16> Enc, string n>: QISAQuantumReg<Enc, n>; 

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
def QPR : RegisterClass<"QISA", [qubit], 32, (add (sequence "QR%u", 0, 255))>;

def QPROpnd : RegisterOperand<QPR>;

// Define IDAddr and UDAddr
