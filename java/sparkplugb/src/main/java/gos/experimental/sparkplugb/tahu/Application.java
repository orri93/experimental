package gos.experimental.sparkplugb.tahu;

import static org.eclipse.tahu.message.model.MetricDataType.*;

import java.math.BigInteger;
import java.util.ArrayList;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Random;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

import javax.net.SocketFactory;
import javax.net.ssl.SSLSocketFactory;

import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.MqttCallbackExtended;
import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;
import org.eclipse.paho.client.mqttv3.MqttPersistenceException;
import org.eclipse.tahu.SparkplugException;
import org.eclipse.tahu.SparkplugInvalidTypeException;
import org.eclipse.tahu.message.SparkplugBPayloadEncoder;
import org.eclipse.tahu.message.model.*;
import org.eclipse.tahu.message.model.DataSet.DataSetBuilder;
import org.eclipse.tahu.message.model.Metric.MetricBuilder;
import org.eclipse.tahu.message.model.PropertySet.PropertySetBuilder;
import org.eclipse.tahu.message.model.Row.RowBuilder;
import org.eclipse.tahu.message.model.SparkplugBPayload.SparkplugBPayloadBuilder;
import org.eclipse.tahu.message.model.Template.TemplateBuilder;
import org.eclipse.tahu.util.CompressionAlgorithm;
import org.eclipse.tahu.util.PayloadUtil;

public class Application implements MqttCallbackExtended {

	// HW/SW versions
	private static final String HW_VERSION = "Emulated Hardware";
	private static final String SW_VERSION = "v1.0.0";

	private static final String NAMESPACE = "spBv1.0";

	// Configuration
	private static final boolean USING_REAL_TLS = false;
	private static final boolean USING_COMPRESSION = false;
	private static final CompressionAlgorithm compressionAlgorithm = CompressionAlgorithm.GZIP;
	private String serverUrl = "tcp://localhost:1883";
	private String groupId = "Sparkplug B Devices";
	private String edgeNode = "Java Sparkplug B Example";
	private String deviceId = "SparkplugBExample";
	private String clientId = null;
	// private String clientId = "SparkplugBExampleEdgeNode";
	private String username = "admin";
	private String password = "changeme";
	private long PUBLISH_PERIOD = 60000; // Publish period in milliseconds
	private ExecutorService executor;
	private MqttClient client;

	private int bdSeq = 0;
	private int seq = 0;

	private Object seqLock = new Object();

	public static void main(String[] args) {
		Application example = new Application();
		example.run();
	}
	
	public void run() {
		try {
			// Random generator and thread pool for outgoing published messages
			executor = Executors.newFixedThreadPool(1);

			// Build up DEATH payload - note DEATH payloads don't have a regular sequence number
			SparkplugBPayloadBuilder deathPayload = new SparkplugBPayloadBuilder().setTimestamp(new Date());
			deathPayload = addBdSeqNum(deathPayload);
			byte[] deathBytes;
			if (USING_COMPRESSION) {
				// Compress payload (optional)
				deathBytes = new SparkplugBPayloadEncoder()
						.getBytes(PayloadUtil.compress(deathPayload.createPayload(), compressionAlgorithm));
			} else {
				deathBytes = new SparkplugBPayloadEncoder().getBytes(deathPayload.createPayload());
			}

			MqttConnectOptions options = new MqttConnectOptions();

			if (USING_REAL_TLS) {
				SocketFactory sf = SSLSocketFactory.getDefault();
				options.setSocketFactory(sf);
			}

			// Connect to the MQTT Server
			options.setAutomaticReconnect(true);
			options.setCleanSession(true);
			options.setConnectionTimeout(30);
			options.setKeepAliveInterval(30);
			options.setUserName(username);
			options.setPassword(password.toCharArray());
			options.setWill(NAMESPACE + "/" + groupId + "/NDEATH/" + edgeNode, deathBytes, 0, false);
			client = new MqttClient(serverUrl, clientId);
			client.setTimeToWait(2000);
			client.setCallback(this); // short timeout on failure to connect
			client.connect(options);

			// Subscribe to control/command messages for both the edge of network node and the attached devices
			client.subscribe(NAMESPACE + "/" + groupId + "/NCMD/" + edgeNode + "/#", 0);
			client.subscribe(NAMESPACE + "/" + groupId + "/DCMD/" + edgeNode + "/#", 0);
			client.subscribe(NAMESPACE + "/#", 0);

			// Loop forever publishing data every PUBLISH_PERIOD
			while (true) {
				Thread.sleep(PUBLISH_PERIOD);

				if (client.isConnected()) {
					synchronized (seqLock) {
						System.out.println("Connected - publishing new data");
						// Create the payload and add some metrics
						SparkplugBPayload payload =
								new SparkplugBPayload(new Date(), newMetrics(false), getSeqNum(), newUUID(), null);

						// Compress payload (optional)
						if (USING_COMPRESSION) {
							client.publish(NAMESPACE + "/" + groupId + "/DDATA/" + edgeNode + "/" + deviceId,
									new SparkplugBPayloadEncoder()
											.getBytes(PayloadUtil.compress(payload, compressionAlgorithm)),
									0, false);
						} else {
							client.publish(NAMESPACE + "/" + groupId + "/DDATA/" + edgeNode + "/" + deviceId,
									new SparkplugBPayloadEncoder().getBytes(payload), 0, false);
						}
					}
				} else {
					System.out.println("Not connected - not publishing data");
				}
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	// Used to add the birth/death sequence number
	private SparkplugBPayloadBuilder addBdSeqNum(SparkplugBPayloadBuilder payload) throws Exception {
		if (payload == null) {
			payload = new SparkplugBPayloadBuilder();
		}
		if (bdSeq == 256) {
			bdSeq = 0;
		}
		payload.addMetric(new MetricBuilder("bdSeq", Int64, (long) bdSeq).createMetric());
		bdSeq++;
		return payload;
	}
	
	// Used to add the sequence number
	private long getSeqNum() throws Exception {
		System.out.println("seq: " + seq);
		if (seq == 256) {
			seq = 0;
		}
		return seq++;
	}

	@Override
	public void connectionLost(Throwable cause) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void messageArrived(String topic, MqttMessage message)
	    throws Exception {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void deliveryComplete(IMqttDeliveryToken token) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void connectComplete(boolean reconnect, String serverURI) {
		// TODO Auto-generated method stub
		
	}
	
	private String newUUID() {
		return java.util.UUID.randomUUID().toString();
	}
	
	private List<Metric> newMetrics(boolean isBirth) throws SparkplugException {
		Random random = new Random();
		List<Metric> metrics = new ArrayList<Metric>();
		metrics.add(new MetricBuilder("Int8", Int8, (byte) random.nextInt()).createMetric());
		metrics.add(new MetricBuilder("Int16", Int16, (short) random.nextInt()).createMetric());
		metrics.add(new MetricBuilder("Int32", Int32, random.nextInt()).createMetric());
		metrics.add(new MetricBuilder("Int64", Int64, random.nextLong()).createMetric());
		metrics.add(new MetricBuilder("UInt8", UInt8, (short) random.nextInt()).createMetric());
		metrics.add(new MetricBuilder("UInt16", UInt16, random.nextInt()).createMetric());
		metrics.add(new MetricBuilder("UInt32", UInt32, random.nextLong()).createMetric());
		metrics.add(new MetricBuilder("UInt64", UInt64, BigInteger.valueOf(random.nextLong())).createMetric());
		metrics.add(new MetricBuilder("Float", Float, random.nextFloat()).createMetric());
		metrics.add(new MetricBuilder("Double", Double, random.nextDouble()).createMetric());
		metrics.add(new MetricBuilder("Boolean", Boolean, random.nextBoolean()).createMetric());
		metrics.add(new MetricBuilder("String", String, newUUID()).createMetric());
		metrics.add(new MetricBuilder("DateTime", DateTime, new Date()).createMetric());
		metrics.add(new MetricBuilder("Text", Text, newUUID()).createMetric());
		metrics.add(new MetricBuilder("UUID", UUID, newUUID()).createMetric());
		// metrics.add(new MetricBuilder("Bytes", Bytes, randomBytes(20)).createMetric());
		// metrics.add(new MetricBuilder("File", File, null).createMetric());

		// DataSet
		metrics.add(new MetricBuilder("DataSet", DataSet, newDataSet()).createMetric());
		if (isBirth) {
			metrics.add(new MetricBuilder("TemplateDef", Template, newTemplate(true, null)).createMetric());
		}

		// Template
		metrics.add(new MetricBuilder("TemplateInst", Template, newTemplate(false, "TemplateDef")).createMetric());

		// Complex Template
		metrics.addAll(newComplexTemplate(isBirth));

		// Metrics with properties
		metrics.add(new MetricBuilder("IntWithProps", Int32, random.nextInt()).properties(
				new PropertySetBuilder().addProperty("engUnit", new PropertyValue(PropertyDataType.String, "My Units"))
						.addProperty("engHigh", new PropertyValue(PropertyDataType.Int32, Integer.MAX_VALUE))
						.addProperty("engLow", new PropertyValue(PropertyDataType.Int32, Integer.MIN_VALUE))
						.createPropertySet())
				.createMetric());

		// Aliased metric
		// The name and alias will be specified in a NBIRTH/DBIRTH message.
		// Only the alias will be specified in a NDATA/DDATA message.
		Long alias = 1111L;
		if (isBirth) {
			metrics.add(new MetricBuilder("AliasedString", String, newUUID()).alias(alias).createMetric());
		} else {
			metrics.add(new MetricBuilder(alias, String, newUUID()).createMetric());
		}

		return metrics;
	}

	private PropertySet newPropertySet() throws SparkplugException {
		return new PropertySetBuilder().addProperties(newProps(true)).createPropertySet();
	}

	private Map<String, PropertyValue> newProps(boolean withPropTypes) throws SparkplugException {
		Random random = new Random();
		Map<String, PropertyValue> propMap = new HashMap<String, PropertyValue>();
		propMap.put("PropInt8", new PropertyValue(PropertyDataType.Int8, (byte) random.nextInt()));
		propMap.put("PropInt16", new PropertyValue(PropertyDataType.Int16, (short) random.nextInt()));
		propMap.put("PropInt32", new PropertyValue(PropertyDataType.Int32, random.nextInt()));
		propMap.put("PropInt64", new PropertyValue(PropertyDataType.Int64, random.nextLong()));
		propMap.put("PropUInt8", new PropertyValue(PropertyDataType.UInt8, (short) random.nextInt()));
		propMap.put("PropUInt16", new PropertyValue(PropertyDataType.UInt16, random.nextInt()));
		propMap.put("PropUInt32", new PropertyValue(PropertyDataType.UInt32, random.nextLong()));
		propMap.put("PropUInt64", new PropertyValue(PropertyDataType.UInt64, BigInteger.valueOf(random.nextLong())));
		propMap.put("PropFloat", new PropertyValue(PropertyDataType.Float, random.nextFloat()));
		propMap.put("PropDouble", new PropertyValue(PropertyDataType.Double, random.nextDouble()));
		propMap.put("PropBoolean", new PropertyValue(PropertyDataType.Boolean, random.nextBoolean()));
		propMap.put("PropString", new PropertyValue(PropertyDataType.String, newUUID()));
		propMap.put("PropDateTime", new PropertyValue(PropertyDataType.DateTime, new Date()));
		propMap.put("PropText", new PropertyValue(PropertyDataType.Text, newUUID()));
		if (withPropTypes) {
			propMap.put("PropPropertySet", new PropertyValue(PropertyDataType.PropertySet,
					new PropertySetBuilder().addProperties(newProps(false)).createPropertySet()));
			List<PropertySet> propsList = new ArrayList<PropertySet>();
			propsList.add(new PropertySetBuilder().addProperties(newProps(false)).createPropertySet());
			propsList.add(new PropertySetBuilder().addProperties(newProps(false)).createPropertySet());
			propsList.add(new PropertySetBuilder().addProperties(newProps(false)).createPropertySet());
			propMap.put("PropPropertySetList", new PropertyValue(PropertyDataType.PropertySetList, propsList));
		}
		return propMap;
	}

	private List<Parameter> newParams() throws SparkplugException {
		Random random = new Random();
		List<Parameter> params = new ArrayList<Parameter>();
		params.add(new Parameter("ParamInt32", ParameterDataType.Int32, random.nextInt()));
		params.add(new Parameter("ParamFloat", ParameterDataType.Float, random.nextFloat()));
		params.add(new Parameter("ParamDouble", ParameterDataType.Double, random.nextDouble()));
		params.add(new Parameter("ParamBoolean", ParameterDataType.Boolean, random.nextBoolean()));
		params.add(new Parameter("ParamString", ParameterDataType.String, newUUID()));
		return params;
	}

	private List<Metric> newComplexTemplate(boolean withTemplateDefs) throws SparkplugInvalidTypeException {
		ArrayList<Metric> metrics = new ArrayList<Metric>();
		if (withTemplateDefs) {

			// Add a new template "subType" definition with two primitive members
			metrics.add(new MetricBuilder("subType", Template,
					new TemplateBuilder().definition(true)
							.addMetric(new MetricBuilder("StringMember", String, "value").createMetric())
							.addMetric(new MetricBuilder("IntegerMember", Int32, 0).createMetric()).createTemplate())
									.createMetric());
			// Add new template "newType" definition that contains an instance of "subType" as a member
			metrics.add(new MetricBuilder("newType", Template,
					new TemplateBuilder().definition(true).addMetric(new MetricBuilder("mySubType", Template,
							new TemplateBuilder().definition(false).templateRef("subType")
									.addMetric(new MetricBuilder("StringMember", String, "value").createMetric())
									.addMetric(new MetricBuilder("IntegerMember", Int32, 0).createMetric())
									.createTemplate()).createMetric())
							.createTemplate()).createMetric());
		}

		// Add an instance of "newType
		metrics.add(new MetricBuilder("myNewType", Template,
				new TemplateBuilder().definition(false).templateRef("newType")
						.addMetric(new MetricBuilder("mySubType", Template,
								new TemplateBuilder().definition(false).templateRef("subType")
										.addMetric(new MetricBuilder("StringMember", String, "myValue").createMetric())
										.addMetric(new MetricBuilder("IntegerMember", Int32, 1).createMetric())
										.createTemplate()).createMetric())
						.createTemplate()).createMetric());

		return metrics;

	}

	private Template newTemplate(boolean isDef, String templatRef) throws SparkplugException {
		Random random = new Random();
		List<Metric> metrics = new ArrayList<Metric>();
		metrics.add(new MetricBuilder("MyInt8", Int8, (byte) random.nextInt()).createMetric());
		metrics.add(new MetricBuilder("MyInt16", Int16, (short) random.nextInt()).createMetric());
		metrics.add(new MetricBuilder("MyInt32", Int32, random.nextInt()).createMetric());
		metrics.add(new MetricBuilder("MyInt64", Int64, random.nextLong()).createMetric());
		metrics.add(new MetricBuilder("MyUInt8", UInt8, (short) random.nextInt()).createMetric());
		metrics.add(new MetricBuilder("MyUInt16", UInt16, random.nextInt()).createMetric());
		metrics.add(new MetricBuilder("MyUInt32", UInt32, random.nextLong()).createMetric());
		metrics.add(new MetricBuilder("MyUInt64", UInt64, BigInteger.valueOf(random.nextLong())).createMetric());
		metrics.add(new MetricBuilder("MyFloat", Float, random.nextFloat()).createMetric());
		metrics.add(new MetricBuilder("MyDouble", Double, random.nextDouble()).createMetric());
		metrics.add(new MetricBuilder("MyBoolean", Boolean, random.nextBoolean()).createMetric());
		metrics.add(new MetricBuilder("MyString", String, newUUID()).createMetric());
		metrics.add(new MetricBuilder("MyDateTime", DateTime, new Date()).createMetric());
		metrics.add(new MetricBuilder("MyText", Text, newUUID()).createMetric());
		metrics.add(new MetricBuilder("MyUUID", UUID, newUUID()).createMetric());

		return new TemplateBuilder().version("v1.0").templateRef(templatRef).definition(isDef)
				.addParameters(newParams()).addMetrics(metrics).createTemplate();
	}

	private DataSet newDataSet() throws SparkplugException {
		Random random = new Random();
		return new DataSetBuilder(14).addColumnName("Int8s").addColumnName("Int16s").addColumnName("Int32s")
				.addColumnName("Int64s").addColumnName("UInt8s").addColumnName("UInt16s").addColumnName("UInt32s")
				.addColumnName("UInt64s").addColumnName("Floats").addColumnName("Doubles").addColumnName("Booleans")
				.addColumnName("Strings").addColumnName("Dates").addColumnName("Texts").addType(DataSetDataType.Int8)
				.addType(DataSetDataType.Int16).addType(DataSetDataType.Int32).addType(DataSetDataType.Int64)
				.addType(DataSetDataType.UInt8).addType(DataSetDataType.UInt16).addType(DataSetDataType.UInt32)
				.addType(DataSetDataType.UInt64).addType(DataSetDataType.Float).addType(DataSetDataType.Double)
				.addType(DataSetDataType.Boolean).addType(DataSetDataType.String).addType(DataSetDataType.DateTime)
				.addType(DataSetDataType.Text)
				.addRow(new RowBuilder().addValue(new Value<Byte>(DataSetDataType.Int8, (byte) random.nextInt()))
						.addValue(new Value<Short>(DataSetDataType.Int16, (short) random.nextInt()))
						.addValue(new Value<Integer>(DataSetDataType.Int32, random.nextInt()))
						.addValue(new Value<Long>(DataSetDataType.Int64, random.nextLong()))
						.addValue(new Value<Short>(DataSetDataType.UInt8, (short) random.nextInt()))
						.addValue(new Value<Integer>(DataSetDataType.UInt16, random.nextInt()))
						.addValue(new Value<Long>(DataSetDataType.UInt32, random.nextLong()))
						.addValue(new Value<BigInteger>(DataSetDataType.UInt64, BigInteger.valueOf(random.nextLong())))
						.addValue(new Value<Float>(DataSetDataType.Float, random.nextFloat()))
						.addValue(new Value<Double>(DataSetDataType.Double, random.nextDouble()))
						.addValue(new Value<Boolean>(DataSetDataType.Boolean, random.nextBoolean()))
						.addValue(new Value<String>(DataSetDataType.String, newUUID()))
						.addValue(new Value<Date>(DataSetDataType.DateTime, new Date()))
						.addValue(new Value<String>(DataSetDataType.Text, newUUID())).createRow())
				.addRow(new RowBuilder().addValue(new Value<Byte>(DataSetDataType.Int8, (byte) random.nextInt()))
						.addValue(new Value<Short>(DataSetDataType.Int16, (short) random.nextInt()))
						.addValue(new Value<Integer>(DataSetDataType.Int32, random.nextInt()))
						.addValue(new Value<Long>(DataSetDataType.Int64, random.nextLong()))
						.addValue(new Value<Short>(DataSetDataType.UInt8, (short) random.nextInt()))
						.addValue(new Value<Integer>(DataSetDataType.UInt16, random.nextInt()))
						.addValue(new Value<Long>(DataSetDataType.UInt32, random.nextLong()))
						.addValue(new Value<BigInteger>(DataSetDataType.UInt64, BigInteger.valueOf(random.nextLong())))
						.addValue(new Value<Float>(DataSetDataType.Float, random.nextFloat()))
						.addValue(new Value<Double>(DataSetDataType.Double, random.nextDouble()))
						.addValue(new Value<Boolean>(DataSetDataType.Boolean, random.nextBoolean()))
						.addValue(new Value<String>(DataSetDataType.String, newUUID()))
						.addValue(new Value<Date>(DataSetDataType.DateTime, new Date()))
						.addValue(new Value<String>(DataSetDataType.Text, newUUID())).createRow())
				.createDataSet();
	}

	private class Publisher implements Runnable {

		private String topic;
		private SparkplugBPayload outboundPayload;

		public Publisher(String topic, SparkplugBPayload outboundPayload) {
			this.topic = topic;
			this.outboundPayload = outboundPayload;
		}

		public void run() {
			try {
				outboundPayload.setTimestamp(new Date());
				SparkplugBPayloadEncoder encoder = new SparkplugBPayloadEncoder();

				// Compress payload (optional)
				if (USING_COMPRESSION) {
					client.publish(topic, encoder.getBytes(PayloadUtil.compress(outboundPayload, compressionAlgorithm)),
							0, false);
				} else {
					client.publish(topic, encoder.getBytes(outboundPayload), 0, false);
				}
			} catch (MqttPersistenceException e) {
				e.printStackTrace();
			} catch (MqttException e) {
				e.printStackTrace();
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
	}

}
