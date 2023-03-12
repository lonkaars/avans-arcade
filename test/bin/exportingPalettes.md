```sh
cat test.src|head -n 46 | awk '{printf "%02x: {0x%x,0x%x,0x%x}\n",NR,$1/2,$2/2,$3/2 }' > hex_palettes
```

