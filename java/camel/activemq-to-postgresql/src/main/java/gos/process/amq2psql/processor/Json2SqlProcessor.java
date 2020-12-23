package gos.process.amq2psql.processor;

import java.util.HashMap;
import java.util.Map;

import org.apache.camel.Exchange;
import org.apache.camel.Message;
import org.apache.camel.Processor;

public class Json2SqlProcessor implements Processor {

	@SuppressWarnings("unchecked")
	@Override
	public void process(Exchange exchange) throws Exception {
		String ts = null;
		Double a = null, b = null, c = null;
		Message in = exchange.getIn();
		Map<String, Object> map = (Map<String, Object>)(in.getBody(HashMap.class));
		if (map != null) {
			if(map.containsKey("ts")) {
				ts = map.get("ts").toString();
			}
			if(map.containsKey("a")) {
				a = (Double)map.get("a");
			}
			if(map.containsKey("b")) {
				b = (Double)map.get("b");
			}
			if(map.containsKey("c")) {
				c = (Double)map.get("c");
			}
			if(ts != null && a != null && b != null && c != null) {
				String sql = String.format(
						"INSERT INTO sandbox(ts, a, b, c) VALUES ('%s', %f, %f, %f)",
						ts, a, b, c);
				Message out = exchange.getOut();
				out.setBody(sql);				
			}
		}
	}
}
