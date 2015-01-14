package Solutions;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

import Models.Consumer;
import Models.Point;
import Models.Problem;
import Models.Warehouse;

public class GreedySolver extends Solver {

	public GreedySolver(Problem p) {

		updateData(p.getWarehouses(), p.getConsumers(), p.getTransportCost(),
				p.getTransportCapacity());
		refreshData();

		refreshWarehouses();

	}

	private Consumer findClosestTo(Point p, List<Point> route) {
		Consumer c = null;
		int minDist = -1;
		for (Consumer curr : consumers) {
			if (route.contains(curr)) {
				continue;
			}
			if (minDist > curr.distance(p) || minDist < 0) {
				c = curr;
				minDist = c.distance(p);
			}
		}

		return c;
	}

	private void refreshWarehouses() {
		boolean flag = false;
		for (int i = 0; i < warehouses.size(); i++) {
			if (!warehouses.get(i).hasTransport()) {
				warehouses.set(i, null);
				flag = true;
			}
		}
		if (flag) {
			warehouses.removeAll(Collections.singleton(null));
		}
	}

	private int getRouteCost(List<Point> route) {
		if (route.size() <= 1) {
			return -1;
		}
		int routeCost = 0;
		for (int i = 1; i < route.size(); i++) {
			routeCost += route.get(i - 1).distance(route.get(i));
		}
		return routeCost;
	}

	private List<Point> getRouteGreedy(Warehouse w) {
		List<Point> result = new ArrayList<Point>();
		result.add(w);
		int cap = transportCapacity;
		Point prev = w;
		Point curr = null;
		while (true) {
			curr = findClosestTo(prev, result);
			if (curr == null || cap - curr.getCost() < 0) {
				result.add(w);
				break;
			}
			cap -= curr.getCost();
			result.add(curr);
			prev = curr;
		}
		return result;
	}

	@Override
	public Solution solve() {
		if (!canSolve()) {
			System.out.println("problem is imposible");
			return null;
		}
		Solution s = new Solution(transportCost, consumers.size());
		while (!s.isSolved()) {
			if (!canSolve() && totalConsumersCost > 0) {
				System.out.println("cant solve no more transport");
				return null;
			}
			if (consumers.size() == 0) {
				return s;
			}
			refreshWarehouses();
			if (warehouses.size() <= 0) {
				System.out.println("no more warehouses");
				return null;
			}

			List<Point> bestRoute = new ArrayList<Point>();
			List<Point> currRoute = null;
			int bestCost = -1;
			int currCost = -1;

			for (int i = 0; i < warehouses.size(); i++) {
				currRoute = getRouteGreedy(warehouses.get(i));
				currCost = getRouteCost(currRoute);
				if (bestCost > currCost || bestCost < 0) {
					bestCost = currCost;
					bestRoute = currRoute;
				}
			}

			if (bestCost < 0) {
				System.out.println("best cost is negative");
				break;
			}

			((Warehouse) bestRoute.get(0)).popTransport();
			for (int i = 1; i < bestRoute.size() - 1; i++) {
				Point c = bestRoute.get(i);
				totalConsumersCost -= c.getCost();
				consumers.remove(c);
			}

			totalWarehouseCapacity -= transportCapacity;
			s.addRoute(bestRoute);

		}
		return s;
	}
}
