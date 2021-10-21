"""
 Copyright (C) Elektrobit Automotive GmbH
 All rights exclusively reserved for Elektrobit Automotive GmbH,
 unless expressly agreed to otherwise.
"""

from rtegraph import graph_gen_utils
from rtegraph.constants import *

RUNNABLE_BSW_PROPERTIES = {'name':"Name", 'entryPoint':"entryPoint"}
DIRECT_CALLERS = "./directCallers/*"
MAPPINGS = "events/*/taskEventMappings/*[executableRef='{}']"
TASK_OF_NAME = "partitions/*/tasks/*[name='{}']"
INTERRUPT_OF_NAME = "partitions/*/Interrupts/*[name='{}']"
EXECLUSIVE_AREAS = "partitions/*/softwareComponents/*/exclusiveAreas/* | partitions/*/bswModules/*/exclusiveAreas/*"
TASK_PROPERTIES = {'name':"Name", 'priority':"priority", 'configuredPriority':"Configured Priority"}
SCHM_TASK_PROPERTIES = {'name':"Name", 'configuredPriority':"Configured Priority"}
EXECLUSIVE_AREA_PROPERTIES = {'name':"Name", 'configuredDataConsistencyMechanism':"Configured DC", 'dataConsistencyMechanism':"Current DC", "../../componentTypeName":"Component", "../../name":"Instance"}
OPTIMIZATION_REASON = "optimizationReason"
RUNS_INSIDE_EA = "./runsInsideExecutableInstances/*"
RUNS_INSIDE = "runs inside"
CAN_ENTER_EA = "./canEnterExecutableInstances/*"
CAN_ENTER = "can enter"
NAME = "name"
MAPPED_TO_TASK = 'mappedToTask'
EVENT_NAME = 'eventName'
TASK_NAME = 'taskName'

def __addexecutable(root, executable_instance_ref, cluster, destnode, connectionlabel = None):
    """
    Add nodes and connections from executable to the destnode.
    Add the nodes of the related tasks and the connection between the executable node and the task node
    Add the nodes of the direct callers and the connection between the executable node and their nodes

    :param root: The root node of the RTE xml model.
    :param executable_instance_ref: The reference of the excutable instance.
    :param cluster: The cluster where the nodes needs to be added.
    :param destnode: The node that will act as the destination for the connection (edge).
    :param connectionlabel: The label that will be on the edge between the nodes.
    """
    executableinstance = graph_gen_utils.get_element_by_id(executable_instance_ref)
    exexutablenode = graph_gen_utils.get_or_create_node(cluster, executableinstance, RUNNABLE_BSW_PROPERTIES, RECORD, LIGHT_BLUE)
    graph_gen_utils.connect(exexutablenode, destnode, cluster, connectionlabel)

    # Find the direct callers to this runnable
    directcallers = executableinstance.findall(DIRECT_CALLERS)
    for directcallerref in directcallers:
        found_direct_callers = True
        __addexecutable(root, directcallerref.text, cluster, exexutablenode, '')
    if len(directcallers) == 0:
        found_direct_callers = False

    # Find the executable mapping task
    __add_mapped_tasks_to_subgraph(root, executable_instance_ref, cluster, exexutablenode, found_direct_callers)


def __add_mapped_tasks_to_subgraph(root, executable_instance_refText, cluster, exexutableNode, found_direct_callers):
    mappings = root.xpath(MAPPINGS.format(executable_instance_refText))
    if len(mappings) != 0:
        for mapping in mappings:
            if mapping.find(MAPPED_TO_TASK).text == 'true':
                task = root.xpath(TASK_OF_NAME.format(mapping.find(TASK_NAME).text))
                if len(task) == 0:
                    # will check if it is mapped to an ISR
                    task = root.xpath(INTERRUPT_OF_NAME.format(mapping.find(TASK_NAME).text))
                taskNode = graph_gen_utils.get_or_create_node(cluster, task[0], SCHM_TASK_PROPERTIES if task[0].find('schMTask').text == 'true' else TASK_PROPERTIES, RECORD, YELLOW)
                eventName = mapping.find(EVENT_NAME).text
            elif not found_direct_callers:
                taskNode = graph_gen_utils.get_or_create_node_with_customized_id('unknownTask' + cluster.id, cluster, shape=RECORD, color='#D2691E', additionaltext='Unknown Task')
                eventName = ''
            graph_gen_utils.connect(taskNode, exexutableNode, cluster, "Event:"+eventName)
    elif not found_direct_callers:
        taskNode = graph_gen_utils.get_or_create_node_with_customized_id('unknownTask' + cluster.id, cluster, shape=RECORD, color='#D2691E', additionaltext='Unknown Task')
        eventName = ''
        graph_gen_utils.connect(taskNode, exexutableNode, cluster, eventName)


def main(args):
    root = graph_gen_utils.get_model_root()
    for exclusivearea in root.xpath(EXECLUSIVE_AREAS):
        ## exclusivearea.getparent().getparent() --> getparent to get the parent
        clusterid = exclusivearea.get('id')
        ## add function to add a cluster, get it and use it
        cluster = graph_gen_utils.get_or_create_cluster(clusterid, exclusivearea.getparent().getparent().find(NAME).text + ' - ' + exclusivearea.find(NAME).text)
        optimization_reason_element = exclusivearea.find(OPTIMIZATION_REASON)
        color = LIGHT_GREEN if optimization_reason_element is None else PINK
        exclusiveareanode = graph_gen_utils.get_or_create_node(cluster, exclusivearea, EXECLUSIVE_AREA_PROPERTIES, None, color)
        if optimization_reason_element is not None:
            exclusiveareaOptimizationReasonNode = graph_gen_utils.get_or_create_node_with_customized_id(clusterid + "_" + OPTIMIZATION_REASON, cluster, optimization_reason_element, {'.':'.'}, RECTANGLE, WHITE, fontcolor=BLUE)
            graph_gen_utils.connect(exclusiveareanode, exclusiveareaOptimizationReasonNode, cluster, color=RED)

        for executable_instance_ref in exclusivearea.findall(RUNS_INSIDE_EA):
            __addexecutable(root, executable_instance_ref.text, cluster, exclusiveareanode, RUNS_INSIDE)

        for executable_instance_ref in exclusivearea.findall(CAN_ENTER_EA):
            __addexecutable(root, executable_instance_ref.text, cluster, exclusiveareanode, CAN_ENTER)

    # Adds the legends
    graph_gen_utils.add_legend('task', YELLOW, RECORD, BLACK)
    graph_gen_utils.add_legend('runnable', LIGHT_BLUE, RECORD, BLACK)
    graph_gen_utils.add_legend('Not optimized exclusive area', LIGHT_GREEN, 'ellipse', BLACK)
    graph_gen_utils.add_legend('Optimized exclusive area', PINK, 'ellipse', BLACK)
    graph_gen_utils.add_legend('Optimization reason', WHITE, RECORD, BLUE)
    graph_gen_utils.add_legend('Unknown Task', BROWN, RECORD, BLACK)

    if args.onePerEA:
        # Generate the graph
        graph_gen_utils.generate_graph_per_cluster("Exclusive Areas", "exclusive-areas")
    else:
        # Generate the graph
        graph_gen_utils.generate_graph("Exclusive Areas", "exclusive-areas")


if __name__=="__main__":
    graph_gen_utils.add_option("-perEA", "--onePerEA", False, "Generates one graph per EA", False)
    args = graph_gen_utils.process_args()
    main(args)
