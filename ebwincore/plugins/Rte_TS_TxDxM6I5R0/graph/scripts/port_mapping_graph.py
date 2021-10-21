"""
 Copyright (C) Elektrobit Automotive GmbH
 All rights exclusively reserved for Elektrobit Automotive GmbH,
 unless expressly agreed to otherwise.

 The script only generates port mapping for intra-ecu connections
"""

from rtegraph import graph_gen_utils
from rtegraph.constants import *

NAME = "name"
SOFTWARE_COMPONENTS = "partitions/*/softwareComponents/* | partitions/*/bswModules/*"
PROVIDE_PORTS = "providePorts/*"
CONNECTED_VDIS = "variableDataInstances/*/connectedVariableDataInstances/*"
GET_R_VDI_BY_ID = "partitions/*/bswModules/*/requirePorts/*/variableDataInstances/*[@id='{}'] | partitions/*/softwareComponents/*/requirePorts/*/variableDataInstances/*[@id='{}']"
CONNECTED_OPIS = "operationInstances/*/intraPartition/* | operationInstances/*/interPartition/*"
OP_ARGS = "../../arguments/*"
GET_R_OPI_BY_ID = "partitions/*/bswModules/*/requirePorts/*/operationInstances/*[@id='{}'] | partitions/*/softwareComponents/*/requirePorts/*/operationInstances/*[@id='{}']"
CONNECTED_MSES = "modeDeclarationGroupInstances/*/intraPartition/* | modeDeclarationGroupInstances/*/interPartition/*"
GET_R_MSE_BY_ID = "partitions/*/bswModules/*/requirePorts/*/modeDeclarationGroupInstances/*[@id='{}'] | partitions/*/softwareComponents/*/requirePorts/*/modeDeclarationGroupInstances/*[@id='{}']"
CONNECTED_TRIGGERS = "triggerInstances/*/intraPartition/* | triggerInstances/*/interPartition/*"
GET_R_TRIGGER_BY_ID = "partitions/*/bswModules/*/requirePorts/*/triggerInstances/*[@id='{}'] | partitions/*/softwareComponents/*/requirePorts/*/triggerInstances/*[@id='{}']"

def __addmapping(root, portcluster, pportnode, pport, connectedr, rportbyid, connectionlabel):
    """
    check the mapping for the pport in the xml model for the connected RPort
    in cases of SR, CS, MSE and TRIGGER
    """
    for refElement in pport.xpath(connectedr):
        ref = refElement.text
        element = root.xpath(rportbyid.format(ref, ref))[0]
        rportelement = element.getparent().getparent()
        rport_properties = {"name":"Name", "../../name":"Component"} if rportelement.find(NAME).text is not None else {"id":"Name", "../../name":"Component"}
        rportnode = graph_gen_utils.get_or_create_node(portcluster, rportelement, rport_properties, None, color=LIGHT_GREEN, fontcolor=BLACK)
        graph_gen_utils.connect(pportnode, rportnode, portcluster, connectionlabel)


def main():
    root = graph_gen_utils.get_model_root()
    for swc in root.xpath(SOFTWARE_COMPONENTS):
        swcname = swc.find(NAME).text
        swcCluster = graph_gen_utils.get_or_create_cluster(swcname, swcname)

        for pport in swc.xpath(PROVIDE_PORTS):
            portname = pport.find(NAME).text if pport.find(NAME).text is not None else pport.get("id")
            portcluster = graph_gen_utils.new_cluster(portname + swcname, portname)
            swcCluster.add_cluster(portcluster)
            pportnode = graph_gen_utils.get_or_create_node(portcluster, pport, {}, None, color=LIGHT_BLUE, additionaltext=portname, fontcolor=BLACK)

            #Sender/Receiver
            __addmapping(root, portcluster, pportnode, pport, CONNECTED_VDIS, GET_R_VDI_BY_ID, 'Sender Receiver')

            #Client/Server
            __addmapping(root, portcluster, pportnode, pport, CONNECTED_OPIS, GET_R_OPI_BY_ID, 'Client Server')

            #MSE
            __addmapping(root, portcluster, pportnode, pport, CONNECTED_MSES, GET_R_MSE_BY_ID, 'Mode Switch')

            #TRIGGER
            __addmapping(root, portcluster, pportnode, pport, CONNECTED_TRIGGERS, GET_R_TRIGGER_BY_ID, 'Trigger')

    # Add legends
    graph_gen_utils.add_legend('Required Port', LIGHT_GREEN, 'ellipse', BLACK)
    graph_gen_utils.add_legend('Provider Port', LIGHT_BLUE, 'ellipse', BLACK)

    # generate graph
    graph_gen_utils.generate_graph("Port Connections", "port-connections")


if __name__=="__main__":
    args = graph_gen_utils.process_args()
    main()
