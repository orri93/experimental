package gos.process.amq2psql.configuration;

public class Json2SqlConfiguration {
	private Json2SqlMessaging messaging;

	public Json2SqlMessaging getMessaging() {
		return messaging;
	}

	public void setMessaging(Json2SqlMessaging messaging) {
		this.messaging = messaging;
	}
}
