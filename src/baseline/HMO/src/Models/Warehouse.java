package Models;

public class Warehouse extends Point {
	private int transports;

	public Warehouse(int x, int y) {
		this.x = x;
		this.y = y;
	}

	public Warehouse(int x, int y, int id) {
		this.x = x;
		this.y = y;
		this.id = id;
	}

	public Warehouse(Warehouse other) {
		this.x = other.x;
		this.y = other.y;
		this.id = other.id;
		this.cost = other.cost;
		this.transports = other.transports;
	}

	public void setTransports(int nTransports) {
		this.transports = nTransports;
	}

	public int getTransports() {
		return transports;
	}

	public boolean popTransport() {
		if (hasTransport()) {
			transports--;
			return true;
		} else {
			return false;
		}
	}

	public boolean hasTransport() {
		if (transports > 0) {
			return true;
		} else {
			return false;
		}
	}

	public void pushTransport(int n) {
		transports += n;
	}

	@Override
	public int hashCode() {
		return 1000 * (this.id + 1);
	}

	@Override
	protected Object clone() throws CloneNotSupportedException {
		return new Warehouse(this);
	}
}
