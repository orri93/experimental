package gos.process.amq2psql.processor;

import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.HashMap;
import java.util.Map;

import org.apache.camel.Exchange;
import org.apache.camel.Message;
import org.apache.camel.Processor;

public class Json2SqlProcessor implements Processor {

	@SuppressWarnings("unchecked")
	@Override
	public void process(Exchange exchange) throws Exception {
		String tss = null;
		Double rh = null, t = null, at = null, dht11 = null;
		Message in = exchange.getIn();
		Map<String, Object> map = (Map<String, Object>)(in.getBody(HashMap.class));
		if (map != null) {
			if(map.containsKey("ts")) {
				tss = map.get("ts").toString();
			} else {
				Long ts = in.getHeader("JMSTimestamp", -1L, Long.class);
				DateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss Z");
				Date tsd = new Date(ts);
				tss = sdf.format(tsd);
			}
			if(map.containsKey("t")) {
				at = (Double)map.get("t");
			}
			if(map.containsKey("dht11")) {
				dht11 = (Double)map.get("dht11");
			}
			if(map.containsKey("RH")) {
				rh = (Double)map.get("RH");
			}
			if(map.containsKey("T")) {
				t = (Double)map.get("T");
			}
			if(tss != null && at != null && dht11 != null && rh != null && t != null) {
				String sql = String.format(
						"INSERT INTO dht(ts, at, status, rh, t) VALUES ('%s', %f, %f, %f, %f)",
						tss, at, dht11, rh, t);
				Message out = exchange.getOut();
				out.setBody(sql);				
			}
		}
	}
}
