package gos.process.amq2psql.processor;

import org.apache.camel.Exchange;
import org.apache.camel.Processor;
import org.springframework.stereotype.Component;

@Component
public class RequestHandlerProcessor implements Processor {
	
	public RequestHandlerProcessor() {
	}

	@Override
	public void process(Exchange exchange) throws Exception {
		
	}
}
