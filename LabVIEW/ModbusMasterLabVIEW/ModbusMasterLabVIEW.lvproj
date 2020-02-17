<?xml version='1.0' encoding='UTF-8'?>
<Project Type="Project" LVVersion="18008000">
	<Property Name="varPersistentID:{0F7B498F-5D77-4BB2-BE0F-0DA87AC23C04}" Type="Ref">/My Computer/ModbusRTU.lvlib/400001</Property>
	<Property Name="varPersistentID:{579CF9D3-FA0A-4681-8753-69CFC3453398}" Type="Ref">/My Computer/ModbusRTU.lvlib/000002</Property>
	<Property Name="varPersistentID:{63B1EB02-78A3-4937-891B-D7513C9A1B96}" Type="Ref">/My Computer/ModbusRTU.lvlib/400002</Property>
	<Property Name="varPersistentID:{70E8311E-8AF7-40EC-B9E8-E836D2E35215}" Type="Ref">/My Computer/ModbusRTU.lvlib/300002</Property>
	<Property Name="varPersistentID:{82F2DF9F-F1BB-4DC6-9C53-126A9AED11BA}" Type="Ref">/My Computer/ModbusRTU.lvlib/100002</Property>
	<Property Name="varPersistentID:{C39191E0-EA96-4BBC-A76D-8BAB90709C66}" Type="Ref">/My Computer/ModbusRTU.lvlib/000001</Property>
	<Property Name="varPersistentID:{F249680D-1FCE-4E60-A66C-890A66731526}" Type="Ref">/My Computer/ModbusRTU.lvlib/300001</Property>
	<Property Name="varPersistentID:{FB262325-7773-4FD7-B7C6-C07CDF99F9C5}" Type="Ref">/My Computer/ModbusRTU.lvlib/100001</Property>
	<Item Name="My Computer" Type="My Computer">
		<Property Name="server.app.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="server.control.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="server.tcp.enabled" Type="Bool">false</Property>
		<Property Name="server.tcp.port" Type="Int">0</Property>
		<Property Name="server.tcp.serviceName" Type="Str">My Computer/VI Server</Property>
		<Property Name="server.tcp.serviceName.default" Type="Str">My Computer/VI Server</Property>
		<Property Name="server.vi.callsEnabled" Type="Bool">true</Property>
		<Property Name="server.vi.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="specify.custom.address" Type="Bool">false</Property>
		<Item Name="ModbusMaster.lvlib" Type="Library" URL="../ModbusMaster.lvlib"/>
		<Item Name="ModbusRTU.lvlib" Type="Library" URL="../ModbusRTU.lvlib"/>
		<Item Name="MosbusMotor.lvlib" Type="Library" URL="../MosbusMotor.lvlib"/>
		<Item Name="Dependencies" Type="Dependencies"/>
		<Item Name="Build Specifications" Type="Build"/>
	</Item>
</Project>
