package gos.process.amq2psql.configuration;

public class Json2SqlMessaging {
	private String prefix;
	private Json2SqlQueues queues;
	
	public String getPrefix() {
		return prefix;
	}
	public void setPrefix(String prefix) {
		this.prefix = prefix;
	}
	
	public Json2SqlQueues getQueues() {
		return queues;
	}
	public void setQueues(Json2SqlQueues queues) {
		this.queues = queues;
	}
}
