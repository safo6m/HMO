package Solutions;

import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;

import Models.Consumer;
import Models.Warehouse;

public abstract class Solver {
	protected List<Warehouse> warehouses;
	protected List<Consumer> consumers;
	protected int transportCost;
	protected int transportCapacity;
	protected int totalConsumersCost;
	protected int totalWarehouseCapacity;

	public abstract Solution solve();

	protected void updateData(List<Warehouse> w, List<Consumer> c, int tCost,
			int tCapacity) {
		warehouses = new ArrayList<Warehouse>(w);
		consumers = new LinkedList<Consumer>(c);
		transportCost = tCost;
		transportCapacity = tCapacity;

		refreshData();
	}

	protected void refreshData() {
		totalConsumersCost = 0;
		totalWarehouseCapacity = 0;

		for (int i = 0; i < warehouses.size(); i++) {
			totalWarehouseCapacity += warehouses.get(i).getTransports()
					* transportCapacity;
		}

		for (int i = 0; i < consumers.size(); i++) {
			totalConsumersCost += consumers.get(i).getCost();
		}
	}

	public boolean canSolve() {
		refreshData();
		if (totalConsumersCost > totalWarehouseCapacity) {
			return false;
		}
		return true;
	}
}
