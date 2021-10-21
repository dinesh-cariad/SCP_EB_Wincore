<?xml version="1.0" encoding="UTF-8"?>

<?module <%= MODULE_NAME %>?>

<xgen xmlns="<%= XGENConstants.XGEN_NS_URI %>" xmlns:xi="http://www.w3.org/2001/XInclude">

    <!-- This is the configuration of module <%= MODULE_NAME %> -->
    <module name="<%= MODULE_NAME %>" version="<%= ((JavaGenContext)argument).gen.getVariable("moduleSoftwareVer", String.class, null) %>">
