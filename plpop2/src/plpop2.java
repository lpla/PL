//Author: Leopoldo Pla

import java.io.*;

class plpop2 {
	public static void main(String[] args) {
		AnalizadorSintacticoSLR aslr;
		AnalizadorLexico al;
		RandomAccessFile entrada = null;
		if (args.length == 1) {
			try {
				entrada = new RandomAccessFile(args[0], "r");
				al = new AnalizadorLexico(entrada);
				aslr = new AnalizadorSintacticoSLR(al);
				aslr.analizar();
			} catch (FileNotFoundException e) {
				System.out.println("Error, fichero no encontrado: " + args[0]);
			}
		} else
			System.out.println("Error, uso: java plpop2 <nomfichero>");
	}
}