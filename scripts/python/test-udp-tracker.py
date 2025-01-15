import socket
import struct

def decode_vicon_udp_stream(data):
    """Decode the UDP stream from VICON tracker."""
    offset = 0

    # Frame Number (4 bytes, unsigned int)
    frame_number, = struct.unpack_from('<I', data, offset)
    offset += 4

    # ItemsInBlock (1 byte, unsigned char)
    items_in_block, = struct.unpack_from('<B', data, offset)
    offset += 1

    print(f"Frame Number: {frame_number}")
    print(f"Items In Block: {items_in_block}")

    for i in range(items_in_block):
        # ItemHeader: ItemID (1 byte, unsigned char)
        item_id, = struct.unpack_from('<B', data, offset)
        offset += 1

        # ItemHeader: ItemDataSize (2 bytes, unsigned short)
        item_data_size, = struct.unpack_from('<H', data, offset)
        offset += 2

        # TrackerObject: ItemName (24 bytes, string)
        item_name = struct.unpack_from('<24s', data, offset)[0].decode('utf-8').rstrip('\x00')
        offset += 24

        # TrackerObject: TransX, TransY, TransZ (8 bytes each, double)
        trans_x, = struct.unpack_from('<d', data, offset)
        offset += 8

        trans_y, = struct.unpack_from('<d', data, offset)
        offset += 8

        trans_z, = struct.unpack_from('<d', data, offset)
        offset += 8

        # TrackerObject: RotX, RotY, RotZ (8 bytes each, double)
        rot_x, = struct.unpack_from('<d', data, offset)
        offset += 8

        rot_y, = struct.unpack_from('<d', data, offset)
        offset += 8

        rot_z, = struct.unpack_from('<d', data, offset)
        offset += 8

        print(f"\nItem {i + 1}:")
        print(f"  Item ID: {item_id}")
        print(f"  Item Data Size: {item_data_size}")
        print(f"  Item Name: {item_name}")
        print(f"  Translation: X={trans_x}, Y={trans_y}, Z={trans_z}")
        print(f"  Rotation: X={rot_x}, Y={rot_y}, Z={rot_z}")

def main():
    # Create a UDP socket
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    # Bind the socket to localhost:51001
    server_address = ('localhost', 51001)
    sock.bind(server_address)

    print("Listening for UDP data on",server_address)

    try:
        while True:
            data, _ = sock.recvfrom(1024)  # Receive UDP packet
            print("\nReceived data:")
            decode_vicon_udp_stream(data)
    except KeyboardInterrupt:
        print("\nExiting...")
    finally:
        sock.close()

if __name__ == "__main__":
    main()
