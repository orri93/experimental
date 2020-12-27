package gos.process.amq2psql;

import java.util.Map;

import javax.sql.DataSource;

import org.apache.camel.Exchange;
import org.apache.camel.builder.RouteBuilder;
import org.apache.camel.model.dataformat.JsonLibrary;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.EnableAutoConfiguration;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.boot.context.properties.ConfigurationProperties;
import org.springframework.stereotype.Component;

import gos.process.amq2psql.configuration.Json2SqlConfiguration;
import gos.process.amq2psql.processor.Json2SqlProcessor;

@SpringBootApplication
@EnableAutoConfiguration
public class GosProcessAmq2PsqlApplication {
	private static final Logger Log =
			LoggerFactory.getLogger(GosProcessAmq2PsqlApplication.class);
	
	public static void main(String[] args) {
		SpringApplication.run(GosProcessAmq2PsqlApplication.class, args);
	}
	
	@Component
	@ConfigurationProperties(prefix = "gos")
	class Amq2PsqlConfiguration extends Json2SqlConfiguration {
	}
	
	@Component
	class Amq2PsqlProcessor extends Json2SqlProcessor {
		@Override
		public void process(Exchange exchange) throws Exception {
			super.process(exchange);
		}
	}

	@Component
	class Amq2PsqlRoute extends RouteBuilder {
		
		@Autowired
		DataSource dataSource;

		public DataSource getDataSource() {
			return dataSource;
		}

		public void setDataSource(DataSource dataSource) {
			this.dataSource = dataSource;
		}

		@Autowired
		private Amq2PsqlProcessor processor;
		
		@Autowired
		private Amq2PsqlConfiguration configuration;

		@Override
		public void configure() throws Exception {
			Log.info("Creating Route for ActiveMQ Queue to PostgreSQL JDBC");
			String from = "activemq:queue:" +
			  configuration.getMessaging().getQueues().getLabview();
			Log.info("From: " + from);
			from(from)
			  .unmarshal().json(JsonLibrary.Gson, Map.class).process(processor)
			  //.to("log:testing");
				.to("jdbc:dataSource");
		}
	}
}
