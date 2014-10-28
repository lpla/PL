//Author: Leopoldo Pla

import java.io.BufferedReader;
import java.io.FileReader;
import java.util.Stack;

public class plpop1 {

	public static void main(String[] args) {
		BufferedReader br;
		try {
			br = new BufferedReader(new FileReader(args[0]));
			String line;
			while ((line = br.readLine()) != null) {
				System.out.println(translateExpression(line));
			}
			br.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	private static String translateExpression(String line) {
		Stack<String> A = new Stack<>();
		Stack<String> B = new Stack<>();

		for (char ch : line.toCharArray()) {
			if (isNumber(ch))
				A.push("" + ch);
			else if (ch == '=') {
				while (!B.isEmpty()) {
					String a2 = A.pop();
					String a1 = A.pop();
					char Y = B.pop().toCharArray()[0];
					A.push(translateOperation(a1, Y, a2));
				}

				return A.pop();

			} else {
				while (true) {
					if (B.isEmpty()) {
						B.push("" + ch);
						break;
					}

					else {
						char Y = B.peek().toCharArray()[0];
						if (precedenceCompare(ch, Y) == 1) {
							B.push("" + ch);
							break;
						} else {
							String a2 = A.pop();
							String a1 = A.pop();
							Y = B.pop().toCharArray()[0];
							A.push(translateOperation(a1, Y, a2));
						}
					}
				}
			}
		}
		return "";
	}

	private static Boolean isNumber(char ch) {
		if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '=')
			return false;
		else
			return true;
	}

	private static int precedenceCompare(char X, char Y) {
		if (X == '+' || X == '-') {
			if (Y == '*' || Y == '/')
				return -1;
			else
				return 0;
		} else {
			if (Y == '*' || Y == '/')
				return 0;
			else
				return 1;
		}
	}

	private static String translateOperation(String a1, char Y, String a2) {
		if (Y == '+')
			return "suma(" + a1 + "," + a2 + ")";
		else if (Y == '-')
			return "resta(" + a1 + "," + a2 + ")";
		else if (Y == '*')
			return "prod(" + a1 + "," + a2 + ")";
		else
			return "div(" + a1 + "," + a2 + ")";
	}

}
