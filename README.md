# digispec

Digispec is an interpreted digital logic simulation library. Logic specifications
are stored in `.digi` files which are interpreted by the diginterpreter.

data flows as follows:
digispec -> digitokenizer -> digiparser -> diginterpreter.

The diginterpreter is a step-by-step interpreter that assumes a propogation delay.

The user can define logic chips using the provided definition keyword.

eg.

```
-- Quad 2-Input AND
-- All inputs uninverted, bit, #USET by default
begin definition SN74LS08

INPUT vcc gnd a0 b0 a1 b1 a2 b2 a3 b3
OUTPUT c0 c1 c2 c3

-- type
TYPE i_vcc base::power::VCC
TYPE i_gnd base::power::GND

-- Requirements
REQUIRE i_vcc i_gnd

begin set o_c0
compare i_a0 i_b0
$0
end o_c0

begin set o_c1
compare i_a1 i_b1
$0
end o_c1

begin set o_c2
compare i_a2 i_b2
$0
end o_c2

begin set o_c3
compare i_a3 i_b3
$0
end o_c3

end SN74LS08
```

# Building and installing

See the [BUILDING](BUILDING.md) document.

# Contributing

See the [CONTRIBUTING](CONTRIBUTING.md) document.
