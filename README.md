# GCP_firewall

Automatically generate GCP shell commands to set firewall that blocks ingress from certain region

## 1. Get the latest IP blacklist

Get the latest IP list [here](https://www.ip2location.com/free/visitor-blocker), and choose the output format as CIDR.

## 2. Build command generator

Note that this program requires C++17 standard, make sure your compiler is not too old

```shell
mkdir build && cd build
cmake .. -G Ninja
ninja
```

## 3. Run and generate commands

```shell
./command_generator -i (input ip list) -id (GCP project id)
```

## 4. Run the generated file in GCP cloud shell

```shell
chmod +x block_china.sh
./block_china.sh
```

## Notes

If you are updating firewall rules, make sure clear the old ones before adding the latest rules.
