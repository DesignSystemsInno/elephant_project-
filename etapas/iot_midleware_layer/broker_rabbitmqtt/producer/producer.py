import pika
connection = pika.BlockingConnection(pika.ConnectionParameters('143.244.204.187', 5672, '/', 
pika.PlainCredentials('cerex_iot', 'guest')))
channel = connection.channel()
channel.basic_publish(exchange='cerex_iot', routing_key='cerex_iot', body='Test!')
print("Runnning Producer Application...")
print(" [x] Sent 'Hello World...!'")
connection.close()