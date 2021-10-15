import pika
connection = pika.BlockingConnection(pika.ConnectionParameters('143.244.204.187', 5672, '/',
pika.PlainCredentials("cerex_iot", "guest")))
channel = connection.channel()

def callback(ch, method, properties, body):
    print(f'{body} is received')
channel.basic_consume(queue="iot_cerex", on_message_callback=callback, auto_ack=True)
channel.start_consuming()
        