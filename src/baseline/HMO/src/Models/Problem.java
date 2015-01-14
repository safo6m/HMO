package Models;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;


public class Problem {
	private List<Consumer> consumers;
	private List<Warehouse> warehouses;
	private int transportCapacity;
	private int transportCost;
	private int totalConsumersCost;

	public Problem(String path) throws NumberFormatException, IOException {
		File f = new File(path);
		BufferedReader reader = null;
		try {
			reader = new BufferedReader(new FileReader(f));
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		}

		int nConsumers = Integer.parseInt(reader.readLine());
		consumers = new ArrayList<Consumer>(nConsumers);

		int nWarehouses = Integer.parseInt(reader.readLine());
		warehouses = new ArrayList<Warehouse>(nWarehouses);

		reader.readLine();

		for (int i = 0; i < nWarehouses; i++) {
			String[] coords = reader.readLine().split("\\s+");
			warehouses.add(new Warehouse(Integer.parseInt(coords[0]), Integer
					.parseInt(coords[1]), i));
		}

		reader.readLine();

		for (int i = 0; i < nConsumers; i++) {
			String[] coords = reader.readLine().split("\\s+");
			consumers.add(new Consumer(Integer.parseInt(coords[0]), Integer
					.parseInt(coords[1]), i));
		}

		reader.readLine();

		transportCapacity = Integer.parseInt(reader.readLine());

		reader.readLine();

		for (int i = 0; i < nWarehouses; i++) {
			int nTransports = Integer.parseInt(reader.readLine())
					/ transportCapacity;
			warehouses.get(i).pushTransport(nTransports);
		}

		reader.readLine();

		totalConsumersCost = 0;
		for (int i = 0; i < nConsumers; i++) {
			int consumerCost = Integer.parseInt(reader.readLine());
			consumers.get(i).setCost(consumerCost);
			totalConsumersCost += consumerCost;
		}

		reader.readLine();

		for (int i = 0; i < nWarehouses; i++) {
			int warehouseCost = Integer.parseInt(reader.readLine());
			warehouses.get(i).setCost(warehouseCost);
		}

		reader.readLine();

		transportCost = Integer.parseInt(reader.readLine());

	}

	public Problem(Problem other) {
		consumers = new ArrayList<Consumer>(other.consumers.size());

		for (Consumer c : other.consumers) {
			consumers.add(new Consumer(c));
		}

		warehouses = new ArrayList<Warehouse>(other.warehouses.size());

		for (Warehouse w : other.warehouses) {
			warehouses.add(new Warehouse(w));
		}
		transportCapacity = other.transportCapacity;
		transportCost = other.transportCost;
		totalConsumersCost = other.totalConsumersCost;
	}

	public List<Warehouse> getWarehouses() {
		if (warehouses.contains(null)) {
			refreshProblem();
		}
		return warehouses;
	}

	public Warehouse[] getWarehousesArray() {
		return (Warehouse[]) getWarehouses().toArray();
	}

	public List<Consumer> getConsumers() {
		if (consumers.contains(null)) {
			refreshProblem();
		}
		return consumers;
	}

	public Consumer[] getConsumersArray() {
		return (Consumer[]) getConsumers().toArray();
	}

	public int getTransportCapacity() {
		return transportCapacity;
	}

	public int getTransportCost() {
		return transportCost;
	}

	public void removeWarehouse(int i) {
		if (i < warehouses.size()) {
			warehouses.set(i, null);
		}
	}

	public boolean isFeasible() {
		int warehouseCapacity = 0;
		for (Warehouse w : warehouses) {
			warehouseCapacity += w.getTransports() * transportCapacity;
		}

		if (warehouseCapacity > totalConsumersCost) {
			return true;
		}
		return false;
	}

	private void refreshProblem() {
		if (warehouses.contains(null)) {
			warehouses.removeAll(Collections.singleton(null));
		}
		if (consumers.contains(null)) {
			consumers.removeAll(Collections.singleton(null));
		}
	}

}
