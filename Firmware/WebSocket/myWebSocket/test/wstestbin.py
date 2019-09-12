import websocket, json
 
ws = websocket.WebSocket()
ws.connect("ws://192.168.0.21:81")
ws.recv_frame() #ping

txtAnswer =ws.recv_frame() #>Connected
print ">"+txtAnswer.data

print "\n----BINARY---"
a =  bytearray([0xde, 0xad, 0xbe, 0xef, 0x55, 0xaa])
ws.send_binary(a)

binAnswer = ws.recv_frame()
print websocket.ABNF.OPCODE_MAP[binAnswer.opcode]
for byte in bytearray(binAnswer.data):
    print hex(byte).upper(),
print("")

print "\n----TEXT---" 
ws.send("Hello world")
txtAnswer = ws.recv_frame()
 
print websocket.ABNF.OPCODE_MAP[txtAnswer.opcode]
print ">"+txtAnswer.data
 


print "\n----JSON TEXT---" 
myJson = {"sensor": "temperature", "identifier":"SENS123456789", "value":10, "timestamp": "20/10/2017 10:10:25"}
ws.send(json.dumps(myJson))
json = ws.recv()
print(json) 

ws.close()