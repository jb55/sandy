{ clangStdenv
, SDL2
, mesa
}:

let
  stdenv = clangStdenv;
in stdenv.mkDerivation rec {
  name = "sandy";
  version = "0.1";

  src = ./.;

  makeFlags = "PREFIX=$(out)";

  buildInputs = [ SDL2 mesa ];

  meta = with stdenv.lib; {
    description = "Sandy fun";
    homepage = "https://jb55.com/sandy";
    maintainers = with maintainers; [ jb55 ];
    license = licenses.unfree;
  };
}
