package gos.process.amq2psql.route;

import org.apache.camel.builder.RouteBuilder;
import org.springframework.stereotype.Component;

@Component
public class RequestHandleRoute extends RouteBuilder {
		
	@Override
	public void configure() throws Exception {
		//ModelCamelContext context = this.getContext();
    //context.setStreamCaching(true);

    
	}
}
