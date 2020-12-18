package gos.process.amq2psql;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.EnableAutoConfiguration;
import org.springframework.boot.autoconfigure.SpringBootApplication;

@SpringBootApplication
@EnableAutoConfiguration
public class GosProcessAmq2PsqlApplication {
	public static void main(String[] args) {
		SpringApplication.run(GosProcessAmq2PsqlApplication.class, args);
	}
}
