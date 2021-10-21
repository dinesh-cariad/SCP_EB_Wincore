"""
 Copyright (C) Elektrobit Automotive GmbH
 All rights exclusively reserved for Elektrobit Automotive GmbH,
 unless expressly agreed to otherwise.
"""

from rtegraph import graph_gen_utils
from rtegraph.constants import *

RUNNABLE_BY_ID = "partitions/*/softwareComponents/*/runnables/*[@id='{}']"
BSW_MOODULE_BY_ID = "partitions/*/bswModules/*/bsnps/*/bswModuleEntities/*[@id='{}']"
RUNNABLE_BSW_PROPERTIES = {'name':"Name", 'entryPoint':"entryPoint"}
TASK_OF_NAME = "partitions/*/tasks/*[name='{}']"
INTERRUPT_OF_NAME = "partitions/*/Interrupts/*[name='{}']"
EVENTS = "events/*"
TASK_PROPERTIES = {'name':"Name", 'priority':"priority", 'configuredPriority':"Configured Priority"}
NAME = "name"
COMPONENT = "Component"
COMPONENT_CLUSTER_PROPERTIES = "{}\nCore: {}\nPartition: {}"
CORE_ID = 'coreId'
MAPPED_TO_TASK = 'mappedToTask'
TASK_EVENT_MAPPING = "taskEventMappings/*"
EVENT_NAME = 'eventName'
EXECUTABLE_REF = 'executableRef'
TASK_NAME = 'taskName'

def __addexecutable(root, executable_instance_ref, cluster, destnode, connectionlabel = None):
    executable_instance = root.xpath(RUNNABLE_BY_ID.format(executable_instance_ref))
    if len(executable_instance) == 0:
        executable_instance = root.xpath(BSW_MOODULE_BY_ID.format(executable_instance_ref))
        swc_bsw = executable_instance[0].getparent().getparent().getparent().getparent() #to get the BSW xml element
    else:
        swc_bsw = executable_instance[0].getparent().getparent() #to get the SWC xml element

    swc_bsw_name = swc_bsw.find(NAME).text
    # if swcBswName == 'ldm': #SWC or BSW to generate the mapping for
    partition = swc_bsw.getparent().getparent()
    subcluster = graph_gen_utils.new_cluster(cluster.id + "_" + COMPONENT, 
                COMPONENT_CLUSTER_PROPERTIES.format(swc_bsw_name, partition.find(CORE_ID).text, partition.find(NAME).text))
    cluster.add_cluster(subcluster)
    exexutable_node = graph_gen_utils.get_or_create_node(subcluster, executable_instance[0], RUNNABLE_BSW_PROPERTIES, RECORD, LIGHT_BLUE)
    graph_gen_utils.connect(exexutable_node, destnode, cluster, connectionlabel)


def __add_mapped_tasks_to_subgraph(root, mapping, cluster, srcnode):
        if mapping.find(MAPPED_TO_TASK).text == 'true':
            task = root.xpath(TASK_OF_NAME.format(mapping.find(TASK_NAME).text))
            if len(task) == 0:
                # will check if it is mapped to an ISR
                task = root.xpath(INTERRUPT_OF_NAME.format(mapping.find(TASK_NAME).text))
            tasknode = graph_gen_utils.get_or_create_node(cluster, task[0], TASK_PROPERTIES, RECORD, YELLOW)
            graph_gen_utils.connect(srcnode, tasknode, cluster, '')
        else:
            tasknode = graph_gen_utils.get_or_create_node_with_customized_id('unknownTask' + cluster.id, cluster, shape=RECORD, color=BROWN, additionaltext='Unknown Task')
        graph_gen_utils.connect(srcnode, tasknode, cluster, '')


def main():
    root = graph_gen_utils.get_model_root()
    i = 0
    for event in root.xpath(EVENTS):
        for mapping in event.xpath(TASK_EVENT_MAPPING):
            eventname = mapping.find(EVENT_NAME).text
            cluster = graph_gen_utils.get_or_create_cluster(str(i), eventname)
            eventNode = graph_gen_utils.get_or_create_node_with_customized_id(eventname + str(i), cluster, mapping.find(EVENT_NAME), {'.':'.'}, None)
            __addexecutable(root, mapping.find(EXECUTABLE_REF).text, cluster, eventNode, "")
            __add_mapped_tasks_to_subgraph(root, mapping, cluster, eventNode)
            i = i + 1

    # Add legends
    graph_gen_utils.add_legend('task', YELLOW, RECORD, BLACK)
    graph_gen_utils.add_legend('runnable', LIGHT_BLUE, RECORD, BLACK)
    graph_gen_utils.add_legend('event', WHITE, 'ellipse', BLACK)

    # generate graph
    graph_gen_utils.generate_graph("Event to task mappings", "event-to-task-mappings")


if __name__=="__main__":
    args = graph_gen_utils.process_args()
    main()
