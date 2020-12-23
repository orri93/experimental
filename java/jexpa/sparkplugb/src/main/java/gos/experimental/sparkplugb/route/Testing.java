package gos.experimental.sparkplugb.route;

import org.apache.camel.builder.RouteBuilder;
import org.springframework.stereotype.Component;

@Component
public class Testing extends RouteBuilder {
	@Override
	public void configure() throws Exception {
		from("timer://runOnce?repeatCount=1&delay=5000").to(
		    "bean:runOnceBean");
	}

}
