//Author: Leopoldo Pla

public class AnalizadorSintacticoDR {

	private AnalizadorLexico lexico;
	private Token token;
	private boolean flag;

	public AnalizadorSintacticoDR(AnalizadorLexico al) {
		lexico = al;
		flag = true;
	}

	public final void Fun() {
		token = lexico.siguienteToken();
		if (token.tipo == Token.INT || token.tipo == Token.FLOAT) {
			if (flag)
				System.out.print("1");
			Tipo();
			emparejar(Token.ID);
			Args();
			Bloque();
		} else
			errorSintaxis(Token.INT, Token.FLOAT);
	}

	private void Bloque() {
		if (token.tipo == Token.LLAVEI) {
			if (flag)
				System.out.print(" 12");
			emparejar(Token.LLAVEI);
			SInstr();
			emparejar(Token.LLAVED);
		} else
			errorSintaxis(Token.LLAVEI);
	}

	private void SInstr() {
		if (token.tipo == Token.LLAVEI || token.tipo == Token.ID
				|| token.tipo == Token.IF || token.tipo == Token.INT
				|| token.tipo == Token.FLOAT) {
			if (flag)
				System.out.print(" 13");
			Instr();
			SInstrp();
		} else {
			errorSintaxis(Token.LLAVEI, Token.INT, Token.FLOAT, Token.IF,
					Token.ID);
		}
	}

	private void SInstrp() {
		if (token.tipo == Token.LLAVEI || token.tipo == Token.ID
				|| token.tipo == Token.IF || token.tipo == Token.INT
				|| token.tipo == Token.FLOAT) {
			if (flag)
				System.out.print(" 14");
			Instr();
			SInstrp();
		} else if (token.tipo == Token.LLAVED) {
			if (flag)
				System.out.print(" 15");
			;
		} else {
			errorSintaxis(Token.LLAVEI, Token.LLAVED, Token.INT, Token.FLOAT,
					Token.IF, Token.ID);
		}
	}

	private void Instr() {
		if (token.tipo == Token.LLAVEI) {
			if (flag)
				System.out.print(" 16");
			Bloque();
		} else if (token.tipo == Token.ID) {
			if (flag)
				System.out.print(" 17");
			emparejar(Token.ID);
			emparejar(Token.OPASIG);
			E();
			emparejar(Token.PYC);
		} else if (token.tipo == Token.IF) {
			if (flag)
				System.out.print(" 18");
			emparejar(Token.IF);
			emparejar(Token.PARI);
			E();
			emparejar(Token.PARD);
			Instr();
		} else if (token.tipo == Token.INT || token.tipo == Token.FLOAT) {
			if (flag)
				System.out.print(" 19");
			Tipo();
			emparejar(Token.ID);
			emparejar(Token.PYC);
		} else
			errorSintaxis(Token.LLAVEI, Token.INT, Token.FLOAT, Token.IF,
					Token.ID);
	}

	private void E() {
		if (token.tipo == Token.NUMENTERO || token.tipo == Token.NUMREAL) {
			if (flag)
				System.out.print(" 20");
			T();
			Ep();
		} else
			errorSintaxis(Token.NUMENTERO, Token.NUMREAL);
	}

	private void Ep() {
		if (token.tipo == Token.OPAS) {
			if (flag)
				System.out.print(" 21");
			emparejar(Token.OPAS);
			T();
			Ep();
		} else if (token.tipo == Token.PYC || token.tipo == Token.PARD) {
			if (flag)
				System.out.print(" 22");
			;
		} else
			errorSintaxis(Token.PARD, Token.PYC, Token.OPAS);
	}

	private void T() {
		if (token.tipo == Token.NUMENTERO) {
			if (flag)
				System.out.print(" 23");
			emparejar(Token.NUMENTERO);
		} else if (token.tipo == Token.NUMREAL) {
			if (flag)
				System.out.print(" 24");
			emparejar(Token.NUMREAL);
		} else
			errorSintaxis(Token.NUMENTERO, Token.NUMREAL);
	}

	private void Args() {
		if (token.tipo == Token.PARI) {
			if (flag)
				System.out.print(" 4");
			emparejar(Token.PARI);
			LArgs();
			emparejar(Token.PARD);
		} else {
			errorSintaxis(Token.PARI);
		}
	}

	private void LArgs() {
		if (token.tipo == Token.INT || token.tipo == Token.FLOAT) {
			if (flag)
				System.out.print(" 5");
			A();
			MasArgs();
		} else if (token.tipo == Token.PARD) {
			if (flag)
				System.out.print(" 6");
			;
		} else {
			errorSintaxis(Token.PARD, Token.INT, Token.FLOAT);
		}
	}

	private void MasArgs() {
		if (token.tipo == Token.COMA) {
			if (flag)
				System.out.print(" 7");
			emparejar(Token.COMA);
			A();
			MasArgs();
		} else if (token.tipo == Token.PARD) {
			if (flag)
				System.out.print(" 8");
			;
		} else
			errorSintaxis(Token.PARD, Token.COMA);
	}

	private void A() {
		if (token.tipo == Token.INT || token.tipo == Token.FLOAT) {
			if (flag)
				System.out.print(" 9");
			Tipo();
			Var();
		} else {
			errorSintaxis(Token.INT, Token.FLOAT);
		}
	}

	private void Var() {
		if (token.tipo == Token.AMP) {
			if (flag)
				System.out.print(" 10");
			emparejar(Token.AMP);
			emparejar(Token.ID);
		} else if (token.tipo == Token.ID) {
			if (flag)
				System.out.print(" 11");
			emparejar(Token.ID);
		} else {
			errorSintaxis(Token.AMP, Token.ID);
		}
	}

	private void Tipo() {
		if (token.tipo == Token.INT) {
			if (flag)
				System.out.print(" 2");
			emparejar(Token.INT);
		} else if (token.tipo == Token.FLOAT) {
			if (flag)
				System.out.print(" 3");
			emparejar(Token.FLOAT);
		} else {
			errorSintaxis(Token.INT, Token.FLOAT);
		}

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
