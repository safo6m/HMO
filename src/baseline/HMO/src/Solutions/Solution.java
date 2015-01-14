package Solutions;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashSet;
import java.util.LinkedList;
import java.util.List;
import java.util.Set;

import Models.Point;
import Models.Warehouse;

public class Solution {
	private int transportCost;
	private List<List<Point>> routes;
	private List<Warehouse> warehouses;
	private Set<Integer> Ids;
	private int totalCost;
	private int totalConsumers;

	public Solution(int transportCost, int nConsumers) {
		this.transportCost = transportCost;
		routes = new LinkedList<List<Point>>();
		warehouses = new LinkedList<Warehouse>();
		Ids = new HashSet<Integer>();
		totalCost = 0;
		totalConsumers = nConsumers;
	}

	public void addRoute(List<Point> route) {
		// System.out.print("Adding route: ");
		// System.out.println(route);
		Warehouse w = (Warehouse) route.get(0);
		warehouses.add(new Warehouse(w));

		if (Ids.add(w.getId())) {
			totalCost += w.getCost();
		}

		totalCost += transportCost;

		route = shortestRoute(route);

		totalCost += routeDistance(route);

		route.remove(route.size() - 1);
		route.remove(0);

		totalConsumers -= route.size();

		routes.add(new LinkedList<Point>(route));

	}

	private int routeDistance(List<Point> route) {
		int sum = 0;
		for (int i = 1; i < route.size(); i++) {
			sum += route.get(i - 1).distance(route.get(i));
		}
		return sum;
	}

	public int cost() {
		return this.totalCost;
	}

	@Override
	public String toString() {
		StringBuilder sb = new StringBuilder();
		sb.append(routes.size()).append("\n\n");

		int nRoutes = routes.size();
		for (int i = 0; i < nRoutes; i++) {
			sb.append(warehouses.get(i).getId()).append(":\t");
			List<Point> route = routes.get(i);
			for (Point p : route) {
				sb.append(p.getId()).append(" ");
			}
			sb.deleteCharAt(sb.length() - 1);
			sb.append("\n\n");
		}

		sb.append("\n").append(totalCost);
		return sb.toString();
	}

	public boolean isSolved() {
		return totalConsumers == 0;
	}

	public Set<Integer> getRoutesIds() {
		Set<Integer> result = new HashSet<Integer>();
		for (List<Point> route : routes) {
			for (Point p : route) {
				result.add(p.getId());
			}
		}

		return result;
	}

	private List<Point> shortestRoute(List<Point> route) {
		List<Point> bestRoute = new ArrayList<Point>(route);
		int bestSore = routeDistance(route);
		for (int i = 0; i < 1000; i++) {
			Point w = route.get(0);
			route.remove(0);
			route.remove(route.size() - 1);

			Collections.shuffle(route);
			route.add(0, w);
			route.add(w);
			int tmpScore = routeDistance(route);
			if (tmpScore < bestSore) {
				bestRoute = new ArrayList<Point>(route);
				bestSore = tmpScore;
			}
		}
		return bestRoute;
	}
}
