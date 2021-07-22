package ProgettoIOT;


class DataPoint {
	private double value;
	private long time;
	private String status;
	
	public DataPoint(double value, long time, String status) {
		this.value = value;
		this.time = time;
		this.status = status;
	}
	
	public double getValue() {
		return value;
	}
	
	public long getTime() {
		return time;
	}
	
	public String getStatus() {
		return status;
	}
}
