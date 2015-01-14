package Main_package;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

import Models.Problem;
import Solutions.GreedySolver2;
import Solutions.Solution;
import Solutions.Solver;

public class Main {

	public static void main(String[] args) {

		BufferedReader reader = new BufferedReader(new InputStreamReader(
				System.in));

		System.out.print("Path to file: ");
		
		String path = null;
		try {
			path = reader.readLine();
		} catch (IOException e1) {
			e1.printStackTrace();
		}

		Problem p = null;
		try {
			p = new Problem(path);
		} catch (NumberFormatException | IOException e) {
			e.printStackTrace();
		}

		Problem currProblem;

		Solver solver = new GreedySolver2(new Problem(p));
		Solution best = solver.solve();

		for (int i = 0; i < p.getWarehouses().size(); i++) {
			for (int j = 0; j < p.getWarehouses().size(); j++) {
				currProblem = new Problem(p);
				currProblem.removeWarehouse(i);
				currProblem.removeWarehouse(j);
				solver = new GreedySolver2(currProblem);
				Solution s = solver.solve();
				if (s != null && s.isSolved() && s.cost() < best.cost()) {
					best = s;
				}
			}
		}

		System.out.println(best);
	}
}
