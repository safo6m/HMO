package Models;

public class Consumer extends Point {

	public Consumer(int x, int y) {
		this.x = x;
		this.y = y;
	}

	public Consumer(int x, int y, int id) {
		this.x = x;
		this.y = y;
		this.id = id;
	}

	public Consumer(Consumer other) {
		this.cost = other.cost;
		this.id = other.id;
		this.x = other.x;
		this.y = other.y;
	}

	@Override
	public int hashCode() {
		return this.id;
	}

	@Override
	protected Object clone() throws CloneNotSupportedException {
		return new Consumer(this);
	}

}
