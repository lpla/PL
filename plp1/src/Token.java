//Author: Leopoldo Pla

public class Token {

	public String lexema;
	public int tipo;
	public int fila;
	public int columna;

	public static final int PARI = 1, PARD = 2, COMA = 3, AMP = 4, LLAVEI = 5,
			LLAVED = 6, OPASIG = 7, PYC = 8, OPAS = 9, INT = 10, FLOAT = 11,
			IF = 12, ID = 13, NUMENTERO = 14, NUMREAL = 15, EOF = 16;

	public String toString() {
		switch (tipo) {
		case PARI:
			return "(";
		case PARD:
			return ")";
		case COMA:
			return ",";
		case AMP:
			return "&";
		case LLAVEI:
			return "{";
		case LLAVED:
			return "}";
		case OPASIG:
			return ":=";
		case PYC:
			return ";";
		case OPAS:
			return "+ -";
		case INT:
			return "'int'";
		case FLOAT:
			return "'float'";
		case IF:
			return "'if'";
		case ID:
			return "identificador";
		case NUMENTERO:
			return "numero entero";
		case NUMREAL:
			return "numero real";
		case EOF:
			return "final de fichero";
		default:

		}
		return "";
	}

	public int getFila() {
		return fila;
	}

	public int getColumna() {
		return columna;
	}

	public String getLexema() {
		return lexema;
	}
}
