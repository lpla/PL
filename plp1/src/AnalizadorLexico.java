//Author: Leopoldo Pla

import java.io.*;
import java.util.LinkedList;
import java.util.Queue;

public class AnalizadorLexico {
	private RandomAccessFile fichero;
	private Token token;
	private String lexema;
	private int filaLectura;
	private int columnaLectura;
	private Queue<Character> buffer;

	public static final char EOF = '$';

	public AnalizadorLexico(RandomAccessFile entrada) {
		fichero = entrada;

		filaLectura = 1;
		columnaLectura = 1;
		buffer = new LinkedList<Character>();
	}

	public char leerCaracter() {
		char currentChar;
		try {
			if (buffer.isEmpty()) {
				currentChar = (char) fichero.readByte();
				columnaLectura++;

				if (currentChar == '\n') {
					filaLectura++;
					columnaLectura = 1;
				}
			} else
				currentChar = buffer.poll();

			return currentChar;

		} catch (EOFException e) {
			return EOF;
		} catch (IOException e) {
		}
		return ' ';
	}

	public Token siguienteToken() {
		token = new Token();
		lexema = "";
		token.fila = filaLectura;
		token.columna = columnaLectura;
		char input = ' ';

		int estado = 1;

		while (estado > 0) {
			if (estado == 1) {
				token.fila = filaLectura;
				token.columna = columnaLectura;
			}

			if (estado == 1 && buffer.size() != 0)
				token.columna -= 1;

			input = leerCaracter();

			estado = delta(estado, input);

			if (input != ' ' && input != '\t' && input != '\n'
					&& buffer.isEmpty())
				lexema += input;

		}

		if (estado == -13) {
			if (lexema.contentEquals("if"))
				estado = -12;
			else if (lexema.contentEquals("int"))
				estado = -10;
			else if (lexema.contentEquals("float"))
				estado = -11;
		}

		token.tipo = estado * -1;

		token.lexema = lexema;

		return token;
	}

	private int delta(int estado, int c) {
		switch (estado) {
		case 1:
			if (c == ' ' || c == '\t' || c == '\n')
				return 1;
			else if (c == '(')
				return -1;
			else if (c == ')')
				return -2;
			else if (c == ',')
				return -3;
			else if (c == '&')
				return -4;
			else if (c == '{')
				return -5;
			else if (c == '}')
				return -6;
			else if (c == ':')
				return 2;
			else if (c == ';')
				return -8;
			else if (c == '+' || c == '-')
				return -9;
			else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
				return 3;
			else if (c >= '0' && c <= '9')
				return 4;
			else if (c == EOF) {
				return -16;
			} else {
				System.err.println("Error lexico (" + (filaLectura) + ","
						+ (columnaLectura - 1 - (buffer.size()))
						+ "): caracter '" + (char) c + "' incorrecto");
				System.exit(-1);
			}
		case 2:
			if (c == '=')
				return -7;
			else {
				System.err.println("Error lexico (" + (token.fila) + ","
						+ (token.columna) + "): caracter ':' incorrecto");
				System.exit(-1);
			}
		case 3:
			if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
					|| (c >= '0' && c <= '9'))
				return 3;
			else
				buffer.add((char) c);
			return -13;
		case 4:
			if (c >= '0' && c <= '9')
				return 4;
			else if (c == '.')
				return 5;
			else
				buffer.add((char) c);
			return -14;

		case 5:
			if (c >= '0' && c <= '9')
				return 6;
			else {
				lexema = lexema.substring(0, lexema.length() - 1);
				buffer.add('.');
				buffer.add((char) c);
				return -14;
			}
		case 6:
			if (c >= '0' && c <= '9')
				return 6;
			else
				buffer.add((char) c);
			return -15;
		default:
		}
		return 0;
	}
}
