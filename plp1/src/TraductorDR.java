//Author: Leopoldo Pla

public class TraductorDR {

	private AnalizadorLexico lexico;
	private Token token;

	public TraductorDR(AnalizadorLexico al) {
		lexico = al;
	}

	public final String Fun() { // Fun → Tipo id Args Bloque
		String tipotrad, idlexema, argstrad;
		Atributos bloquetrad = new Atributos();

		token = lexico.siguienteToken();

		tipotrad = Tipo();
		idlexema = token.lexema;
		emparejar(Token.ID);
		argstrad = Args();
		bloquetrad = Bloque(0);

		return "function " + idlexema + argstrad + tipotrad + ";\n" + "var "
				+ bloquetrad.variables + bloquetrad.traduccion;

	}

	private Atributos Bloque(int ph) {
		if (token.tipo == Token.LLAVEI) {
			Atributos sinstrtrad;
			emparejar(Token.LLAVEI);
			sinstrtrad = SInstr(ph);
			emparejar(Token.LLAVED);

			Atributos result = new Atributos();
			result.variables = sinstrtrad.variables;
			result.traduccion = "begin\n" + sinstrtrad.traduccion + "end";

			return result;
		} else
			errorSintaxis(Token.LLAVEI);

		return new Atributos();
	}

	private Atributos SInstr(int ph) {
		if (token.tipo == Token.LLAVEI || token.tipo == Token.ID
				|| token.tipo == Token.IF || token.tipo == Token.INT
				|| token.tipo == Token.FLOAT) {

			Atributos instrtrad, sinstrptrad;
			Atributos result = new Atributos();
			instrtrad = Instr(ph);
			sinstrptrad = SInstrp(ph);

			result.variables = instrtrad.variables + sinstrptrad.variables;

			if (instrtrad.traduccion != "" && sinstrptrad.traduccion != "")
				result.traduccion = instrtrad.traduccion + ";\n"
						+ sinstrptrad.traduccion;
			else if (instrtrad.traduccion == "" && sinstrptrad.traduccion != "")
				result.traduccion = sinstrptrad.traduccion;
			else if (instrtrad.traduccion != "" && sinstrptrad.traduccion == "")
				result.traduccion = instrtrad.traduccion + "\n";
			else if (instrtrad.traduccion == "" && sinstrptrad.traduccion == "")
				result.traduccion = "noinstruccion\n";

			return result;
		} else {
			errorSintaxis(Token.LLAVEI, Token.INT, Token.FLOAT, Token.IF,
					Token.ID);
		}

		return new Atributos();
	}

	private Atributos SInstrp(int ph) {
		if (token.tipo == Token.LLAVEI || token.tipo == Token.ID
				|| token.tipo == Token.IF || token.tipo == Token.INT
				|| token.tipo == Token.FLOAT) {

			Atributos instrtrad, sinstrptrad;
			Atributos result = new Atributos();
			instrtrad = Instr(ph);
			sinstrptrad = SInstrp(ph);

			result.variables = instrtrad.variables + sinstrptrad.variables;

			if (instrtrad.traduccion != "" && sinstrptrad.traduccion != "")
				result.traduccion = instrtrad.traduccion + ";\n"
						+ sinstrptrad.traduccion;
			else if (instrtrad.traduccion == "" && sinstrptrad.traduccion != "")
				result.traduccion = sinstrptrad.traduccion;
			else if (instrtrad.traduccion != "" && sinstrptrad.traduccion == "")
				result.traduccion = instrtrad.traduccion + "\n";
			else if (instrtrad.traduccion == "" && sinstrptrad.traduccion == "")
				result.traduccion = "noinstruccion\n";

			return result;
		} else if (token.tipo == Token.LLAVED) {
			return new Atributos();
		} else {
			errorSintaxis(Token.LLAVEI, Token.INT, Token.FLOAT,
					Token.IF, Token.ID);
		}

		return new Atributos();
	}

	private Atributos Instr(int ph) {
		if (token.tipo == Token.LLAVEI) {
			Atributos bloquetrad;
			bloquetrad = Bloque(ph + 1);
			return bloquetrad;
		} else if (token.tipo == Token.ID) {
			String idlexema, opasiglexema, etrad;
			idlexema = token.lexema;
			emparejar(Token.ID);
			opasiglexema = token.lexema;
			emparejar(Token.OPASIG);
			etrad = E();
			emparejar(Token.PYC);

			Atributos result = new Atributos();
			result.traduccion = idlexema + " " + opasiglexema + " " + etrad;
			return result;
		} else if (token.tipo == Token.IF) {
			Atributos instrtrad;
			String etrad;
			Atributos result = new Atributos();
			emparejar(Token.IF);
			emparejar(Token.PARI);
			etrad = E();
			emparejar(Token.PARD);
			instrtrad = Instr(ph);

			result.variables = instrtrad.variables;
			if (instrtrad.traduccion == "")
				result.traduccion = "if " + etrad + " then\n" + "noinstruccion";

			else
				result.traduccion = "if " + etrad + " then\n"
						+ instrtrad.traduccion;

			return result;

		} else if (token.tipo == Token.INT || token.tipo == Token.FLOAT) {
			String tipotrad, idlexema;
			tipotrad = Tipo();
			idlexema = token.lexema;
			emparejar(Token.ID);
			emparejar(Token.PYC);

			Atributos result = new Atributos();

			if (ph != 0)
				result.variables = "b" + ph + "_";

			result.variables += idlexema + tipotrad + ";" + "\n";
			return result;
		} else
			errorSintaxis(Token.LLAVEI, Token.INT, Token.FLOAT, Token.IF,
					Token.ID);

		return new Atributos();
	}

	private String E() {
		if (token.tipo == Token.NUMENTERO || token.tipo == Token.NUMREAL) {
			Atributos ttrad;
			String eptrad;
			ttrad = T();
			eptrad = Ep(ttrad);

			return eptrad;
		} else
			errorSintaxis(Token.NUMENTERO, Token.NUMREAL);

		return "";
	}

	private String Ep(Atributos ph) {
		String signo = token.lexema;
		if (token.tipo == Token.OPAS) {
			Atributos ttrad;
			Atributos phep = new Atributos();
			emparejar(Token.OPAS);
			ttrad = T();

			if ((ph.tipo == "numentero") && (ttrad.tipo == "numreal")) {
				phep.tipo = "numreal";
				phep.traduccion = "itor(" + ph.traduccion + ") " + signo + "r "
						+ ttrad.traduccion;
			} else if ((ph.tipo == "numreal") && (ttrad.tipo == "numentero")) {
				phep.tipo = "numreal";
				phep.traduccion = ph.traduccion + " " + signo + "r itor("
						+ ttrad.traduccion + ")";
			} else if ((ph.tipo == "numentero") && (ttrad.tipo == "numentero")) {
				phep.tipo = "numentero";
				phep.traduccion = ph.traduccion + " " + signo + "i " + ttrad.traduccion;
			} else {
				phep.tipo = "numreal";
				phep.traduccion = ph.traduccion + " " + signo + "r " + ttrad.traduccion;
			}

			return Ep(phep);

		} else if (token.tipo == Token.PYC || token.tipo == Token.PARD) {
			return ph.traduccion;
		} else
			errorSintaxis(Token.PARD, Token.PYC, Token.OPAS);

		return "";
	}

	private Atributos T() {
		Atributos numero = new Atributos();
		if (token.tipo == Token.NUMENTERO) {
			String numenterolexema;
			numenterolexema = token.lexema;
			emparejar(Token.NUMENTERO);
			numero.tipo = "numentero";
			numero.traduccion = numenterolexema;
			return numero;
		} else if (token.tipo == Token.NUMREAL) {
			String numreallexema;
			numreallexema = token.lexema;
			emparejar(Token.NUMREAL);
			numero.tipo = "numreal";
			numero.traduccion = numreallexema;
			return numero;
		} else
			errorSintaxis(Token.NUMENTERO, Token.NUMREAL);

		return new Atributos();
	}

	private String Args() {
		if (token.tipo == Token.PARI) {
			String largstrad;
			emparejar(Token.PARI);
			largstrad = LArgs();
			emparejar(Token.PARD);
			if (largstrad == "")
				return largstrad;
			else
				return "(" + largstrad + ")";
		} else {
			errorSintaxis(Token.PARI);
		}
		return "";
	}

	private String LArgs() {
		if (token.tipo == Token.INT || token.tipo == Token.FLOAT) {
			String atrad, masargstrad;
			atrad = A();
			masargstrad = MasArgs();

			return atrad + masargstrad;
		} else if (token.tipo == Token.PARD) {
			return "";
		} else {
			errorSintaxis(Token.PARD, Token.INT, Token.FLOAT);
		}
		return "";
	}

	private String MasArgs() {
		if (token.tipo == Token.COMA) {
			String atrad, masargstrad;

			emparejar(Token.COMA);
			atrad = A();
			masargstrad = MasArgs();

			return ";" + atrad + masargstrad;
		} else if (token.tipo == Token.PARD) {
			return "";
		} else
			errorSintaxis(Token.PARD, Token.COMA);

		return "";
	}

	private String A() {
		if (token.tipo == Token.INT || token.tipo == Token.FLOAT) {
			String tipotrad, vartrad;
			tipotrad = Tipo();
			vartrad = Var();

			return vartrad + tipotrad;
		} else {
			errorSintaxis(Token.INT, Token.FLOAT);
		}
		return "";
	}

	private String Var() {
		if (token.tipo == Token.AMP) {
			String idlexema;
			emparejar(Token.AMP);
			idlexema = token.lexema;
			emparejar(Token.ID);

			return "var " + idlexema;
		} else if (token.tipo == Token.ID) {
			String idlexema;
			idlexema = token.lexema;
			emparejar(Token.ID);
			return idlexema;
		} else {
			errorSintaxis(Token.AMP, Token.ID);
		}
		return "";
	}

	private String Tipo() {
		if (token.tipo == Token.INT) {
			emparejar(Token.INT);
			return ":integer";

		} else if (token.tipo == Token.FLOAT) {
			emparejar(Token.FLOAT);
			return ":real";
		} else {
			errorSintaxis(Token.INT, Token.FLOAT);
		}
		return "";

	}

	public final void emparejar(int tokEsperado) {
		if (token.tipo == tokEsperado)
			token = lexico.siguienteToken();
		else
			errorSintaxis(tokEsperado);
	}

	private void errorSintaxis(int... tokEsperado) {
		if (token.tipo == Token.EOF)
			System.err
					.print("Error sintactico: encontrado el final del fichero, esperaba");
		else
			System.err.print("Error sintactico (" + token.fila + ","
					+ token.columna + "): encontrado '" + token.lexema + "'"
					+ ", esperaba");
		for (int tok : tokEsperado) {
			Token tmp = new Token();
			tmp.tipo = tok;
			System.err.print(" " + tmp.toString());
		}

		System.err.println();
		System.exit(-1);
	}

	public void comprobarFinFichero() {
		if (token.tipo != Token.EOF)
			errorSintaxis(Token.EOF);
	}
}
