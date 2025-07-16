# Creating Vivado Project and Exporting Platform to Vitis

In the TCL console in Vivado, source the tcl script in the `vivado` directory:

```tcl
cd vivado
source create-vivado-project.tcl
```

This will create a Vivado project, with all the necessary IP cores placed and connected. Then you can manually generate block design and export the hardware platform to Vitis.

You can also use the pre-built `xsa` files provided in the `export` directory.


# Creating Vitis Platform

In the `vitis` directory, you can run the Python script in Vitis Python Command Line Interface to create the Vitis platform:

```bash
cd vitis
vitis -i # assuming vitis is in your PATH

# Inside Vitis Python CLI
run create-vitis-platform.py
```

For most cases in the tutorial, the pre-configured platform in this directory should be sufficient. However, you may want to modify the device tree or other configurations.
