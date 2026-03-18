package org.apache.arrow.flight.impl;

import static io.grpc.MethodDescriptor.generateFullMethodName;

/**
 * <pre>
 * A flight service is an endpoint for retrieving or storing Arrow data. A
 * flight service can expose one or more predefined endpoints that can be
 * accessed using the Arrow Flight Protocol. Additionally, a flight service
 * can expose a set of actions that are available.
 * </pre>
 */
@javax.annotation.Generated(
    value = "by gRPC proto compiler (version 1.63.0)",
    comments = "Source: Flight.proto")
@io.grpc.stub.annotations.GrpcGenerated
public final class FlightServiceGrpc {

  private FlightServiceGrpc() {}

  public static final java.lang.String SERVICE_NAME = "arrow.flight.protocol.FlightService";

  // Static method descriptors that strictly reflect the proto.
  private static volatile io.grpc.MethodDescriptor<org.apache.arrow.flight.impl.Flight.HandshakeRequest,
      org.apache.arrow.flight.impl.Flight.HandshakeResponse> getHandshakeMethod;

  @io.grpc.stub.annotations.RpcMethod(
      fullMethodName = SERVICE_NAME + '/' + "Handshake",
      requestType = org.apache.arrow.flight.impl.Flight.HandshakeRequest.class,
      responseType = org.apache.arrow.flight.impl.Flight.HandshakeResponse.class,
      methodType = io.grpc.MethodDescriptor.MethodType.BIDI_STREAMING)
  public static io.grpc.MethodDescriptor<org.apache.arrow.flight.impl.Flight.HandshakeRequest,
      org.apache.arrow.flight.impl.Flight.HandshakeResponse> getHandshakeMethod() {
    io.grpc.MethodDescriptor<org.apache.arrow.flight.impl.Flight.HandshakeRequest, org.apache.arrow.flight.impl.Flight.HandshakeResponse> getHandshakeMethod;
    if ((getHandshakeMethod = FlightServiceGrpc.getHandshakeMethod) == null) {
      synchronized (FlightServiceGrpc.class) {
        if ((getHandshakeMethod = FlightServiceGrpc.getHandshakeMethod) == null) {
          FlightServiceGrpc.getHandshakeMethod = getHandshakeMethod =
              io.grpc.MethodDescriptor.<org.apache.arrow.flight.impl.Flight.HandshakeRequest, org.apache.arrow.flight.impl.Flight.HandshakeResponse>newBuilder()
              .setType(io.grpc.MethodDescriptor.MethodType.BIDI_STREAMING)
              .setFullMethodName(generateFullMethodName(SERVICE_NAME, "Handshake"))
              .setSampledToLocalTracing(true)
              .setRequestMarshaller(io.grpc.protobuf.ProtoUtils.marshaller(
                  org.apache.arrow.flight.impl.Flight.HandshakeRequest.getDefaultInstance()))
              .setResponseMarshaller(io.grpc.protobuf.ProtoUtils.marshaller(
                  org.apache.arrow.flight.impl.Flight.HandshakeResponse.getDefaultInstance()))
              .setSchemaDescriptor(new FlightServiceMethodDescriptorSupplier("Handshake"))
              .build();
        }
      }
    }
    return getHandshakeMethod;
  }

  private static volatile io.grpc.MethodDescriptor<org.apache.arrow.flight.impl.Flight.Criteria,
      org.apache.arrow.flight.impl.Flight.FlightInfo> getListFlightsMethod;

  @io.grpc.stub.annotations.RpcMethod(
      fullMethodName = SERVICE_NAME + '/' + "ListFlights",
      requestType = org.apache.arrow.flight.impl.Flight.Criteria.class,
      responseType = org.apache.arrow.flight.impl.Flight.FlightInfo.class,
      methodType = io.grpc.MethodDescriptor.MethodType.SERVER_STREAMING)
  public static io.grpc.MethodDescriptor<org.apache.arrow.flight.impl.Flight.Criteria,
      org.apache.arrow.flight.impl.Flight.FlightInfo> getListFlightsMethod() {
    io.grpc.MethodDescriptor<org.apache.arrow.flight.impl.Flight.Criteria, org.apache.arrow.flight.impl.Flight.FlightInfo> getListFlightsMethod;
    if ((getListFlightsMethod = FlightServiceGrpc.getListFlightsMethod) == null) {
      synchronized (FlightServiceGrpc.class) {
        if ((getListFlightsMethod = FlightServiceGrpc.getListFlightsMethod) == null) {
          FlightServiceGrpc.getListFlightsMethod = getListFlightsMethod =
              io.grpc.MethodDescriptor.<org.apache.arrow.flight.impl.Flight.Criteria, org.apache.arrow.flight.impl.Flight.FlightInfo>newBuilder()
              .setType(io.grpc.MethodDescriptor.MethodType.SERVER_STREAMING)
              .setFullMethodName(generateFullMethodName(SERVICE_NAME, "ListFlights"))
              .setSampledToLocalTracing(true)
              .setRequestMarshaller(io.grpc.protobuf.ProtoUtils.marshaller(
                  org.apache.arrow.flight.impl.Flight.Criteria.getDefaultInstance()))
              .setResponseMarshaller(io.grpc.protobuf.ProtoUtils.marshaller(
                  org.apache.arrow.flight.impl.Flight.FlightInfo.getDefaultInstance()))
              .setSchemaDescriptor(new FlightServiceMethodDescriptorSupplier("ListFlights"))
              .build();
        }
      }
    }
    return getListFlightsMethod;
  }

  private static volatile io.grpc.MethodDescriptor<org.apache.arrow.flight.impl.Flight.FlightDescriptor,
      org.apache.arrow.flight.impl.Flight.FlightInfo> getGetFlightInfoMethod;

  @io.grpc.stub.annotations.RpcMethod(
      fullMethodName = SERVICE_NAME + '/' + "GetFlightInfo",
      requestType = org.apache.arrow.flight.impl.Flight.FlightDescriptor.class,
      responseType = org.apache.arrow.flight.impl.Flight.FlightInfo.class,
      methodType = io.grpc.MethodDescriptor.MethodType.UNARY)
  public static io.grpc.MethodDescriptor<org.apache.arrow.flight.impl.Flight.FlightDescriptor,
      org.apache.arrow.flight.impl.Flight.FlightInfo> getGetFlightInfoMethod() {
    io.grpc.MethodDescriptor<org.apache.arrow.flight.impl.Flight.FlightDescriptor, org.apache.arrow.flight.impl.Flight.FlightInfo> getGetFlightInfoMethod;
    if ((getGetFlightInfoMethod = FlightServiceGrpc.getGetFlightInfoMethod) == null) {
      synchronized (FlightServiceGrpc.class) {
        if ((getGetFlightInfoMethod = FlightServiceGrpc.getGetFlightInfoMethod) == null) {
          FlightServiceGrpc.getGetFlightInfoMethod = getGetFlightInfoMethod =
              io.grpc.MethodDescriptor.<org.apache.arrow.flight.impl.Flight.FlightDescriptor, org.apache.arrow.flight.impl.Flight.FlightInfo>newBuilder()
              .setType(io.grpc.MethodDescriptor.MethodType.UNARY)
              .setFullMethodName(generateFullMethodName(SERVICE_NAME, "GetFlightInfo"))
              .setSampledToLocalTracing(true)
              .setRequestMarshaller(io.grpc.protobuf.ProtoUtils.marshaller(
                  org.apache.arrow.flight.impl.Flight.FlightDescriptor.getDefaultInstance()))
              .setResponseMarshaller(io.grpc.protobuf.ProtoUtils.marshaller(
                  org.apache.arrow.flight.impl.Flight.FlightInfo.getDefaultInstance()))
              .setSchemaDescriptor(new FlightServiceMethodDescriptorSupplier("GetFlightInfo"))
              .build();
        }
      }
    }
    return getGetFlightInfoMethod;
  }

  private static volatile io.grpc.MethodDescriptor<org.apache.arrow.flight.impl.Flight.FlightDescriptor,
      org.apache.arrow.flight.impl.Flight.PollInfo> getPollFlightInfoMethod;

  @io.grpc.stub.annotations.RpcMethod(
      fullMethodName = SERVICE_NAME + '/' + "PollFlightInfo",
      requestType = org.apache.arrow.flight.impl.Flight.FlightDescriptor.class,
      responseType = org.apache.arrow.flight.impl.Flight.PollInfo.class,
      methodType = io.grpc.MethodDescriptor.MethodType.UNARY)
  public static io.grpc.MethodDescriptor<org.apache.arrow.flight.impl.Flight.FlightDescriptor,
      org.apache.arrow.flight.impl.Flight.PollInfo> getPollFlightInfoMethod() {
    io.grpc.MethodDescriptor<org.apache.arrow.flight.impl.Flight.FlightDescriptor, org.apache.arrow.flight.impl.Flight.PollInfo> getPollFlightInfoMethod;
    if ((getPollFlightInfoMethod = FlightServiceGrpc.getPollFlightInfoMethod) == null) {
      synchronized (FlightServiceGrpc.class) {
        if ((getPollFlightInfoMethod = FlightServiceGrpc.getPollFlightInfoMethod) == null) {
          FlightServiceGrpc.getPollFlightInfoMethod = getPollFlightInfoMethod =
              io.grpc.MethodDescriptor.<org.apache.arrow.flight.impl.Flight.FlightDescriptor, org.apache.arrow.flight.impl.Flight.PollInfo>newBuilder()
              .setType(io.grpc.MethodDescriptor.MethodType.UNARY)
              .setFullMethodName(generateFullMethodName(SERVICE_NAME, "PollFlightInfo"))
              .setSampledToLocalTracing(true)
              .setRequestMarshaller(io.grpc.protobuf.ProtoUtils.marshaller(
                  org.apache.arrow.flight.impl.Flight.FlightDescriptor.getDefaultInstance()))
              .setResponseMarshaller(io.grpc.protobuf.ProtoUtils.marshaller(
                  org.apache.arrow.flight.impl.Flight.PollInfo.getDefaultInstance()))
              .setSchemaDescriptor(new FlightServiceMethodDescriptorSupplier("PollFlightInfo"))
              .build();
        }
      }
    }
    return getPollFlightInfoMethod;
  }

  private static volatile io.grpc.MethodDescriptor<org.apache.arrow.flight.impl.Flight.FlightDescriptor,
      org.apache.arrow.flight.impl.Flight.SchemaResult> getGetSchemaMethod;

  @io.grpc.stub.annotations.RpcMethod(
      fullMethodName = SERVICE_NAME + '/' + "GetSchema",
      requestType = org.apache.arrow.flight.impl.Flight.FlightDescriptor.class,
      responseType = org.apache.arrow.flight.impl.Flight.SchemaResult.class,
      methodType = io.grpc.MethodDescriptor.MethodType.UNARY)
  public static io.grpc.MethodDescriptor<org.apache.arrow.flight.impl.Flight.FlightDescriptor,
      org.apache.arrow.flight.impl.Flight.SchemaResult> getGetSchemaMethod() {
    io.grpc.MethodDescriptor<org.apache.arrow.flight.impl.Flight.FlightDescriptor, org.apache.arrow.flight.impl.Flight.SchemaResult> getGetSchemaMethod;
    if ((getGetSchemaMethod = FlightServiceGrpc.getGetSchemaMethod) == null) {
      synchronized (FlightServiceGrpc.class) {
        if ((getGetSchemaMethod = FlightServiceGrpc.getGetSchemaMethod) == null) {
          FlightServiceGrpc.getGetSchemaMethod = getGetSchemaMethod =
              io.grpc.MethodDescriptor.<org.apache.arrow.flight.impl.Flight.FlightDescriptor, org.apache.arrow.flight.impl.Flight.SchemaResult>newBuilder()
              .setType(io.grpc.MethodDescriptor.MethodType.UNARY)
              .setFullMethodName(generateFullMethodName(SERVICE_NAME, "GetSchema"))
              .setSampledToLocalTracing(true)
              .setRequestMarshaller(io.grpc.protobuf.ProtoUtils.marshaller(
                  org.apache.arrow.flight.impl.Flight.FlightDescriptor.getDefaultInstance()))
              .setResponseMarshaller(io.grpc.protobuf.ProtoUtils.marshaller(
                  org.apache.arrow.flight.impl.Flight.SchemaResult.getDefaultInstance()))
              .setSchemaDescriptor(new FlightServiceMethodDescriptorSupplier("GetSchema"))
              .build();
        }
      }
    }
    return getGetSchemaMethod;
  }

  private static volatile io.grpc.MethodDescriptor<org.apache.arrow.flight.impl.Flight.Ticket,
      org.apache.arrow.flight.impl.Flight.FlightData> getDoGetMethod;

  @io.grpc.stub.annotations.RpcMethod(
      fullMethodName = SERVICE_NAME + '/' + "DoGet",
      requestType = org.apache.arrow.flight.impl.Flight.Ticket.class,
      responseType = org.apache.arrow.flight.impl.Flight.FlightData.class,
      methodType = io.grpc.MethodDescriptor.MethodType.SERVER_STREAMING)
  public static io.grpc.MethodDescriptor<org.apache.arrow.flight.impl.Flight.Ticket,
      org.apache.arrow.flight.impl.Flight.FlightData> getDoGetMethod() {
    io.grpc.MethodDescriptor<org.apache.arrow.flight.impl.Flight.Ticket, org.apache.arrow.flight.impl.Flight.FlightData> getDoGetMethod;
    if ((getDoGetMethod = FlightServiceGrpc.getDoGetMethod) == null) {
      synchronized (FlightServiceGrpc.class) {
        if ((getDoGetMethod = FlightServiceGrpc.getDoGetMethod) == null) {
          FlightServiceGrpc.getDoGetMethod = getDoGetMethod =
              io.grpc.MethodDescriptor.<org.apache.arrow.flight.impl.Flight.Ticket, org.apache.arrow.flight.impl.Flight.FlightData>newBuilder()
              .setType(io.grpc.MethodDescriptor.MethodType.SERVER_STREAMING)
              .setFullMethodName(generateFullMethodName(SERVICE_NAME, "DoGet"))
              .setSampledToLocalTracing(true)
              .setRequestMarshaller(io.grpc.protobuf.ProtoUtils.marshaller(
                  org.apache.arrow.flight.impl.Flight.Ticket.getDefaultInstance()))
              .setResponseMarshaller(io.grpc.protobuf.ProtoUtils.marshaller(
                  org.apache.arrow.flight.impl.Flight.FlightData.getDefaultInstance()))
              .setSchemaDescriptor(new FlightServiceMethodDescriptorSupplier("DoGet"))
              .build();
        }
      }
    }
    return getDoGetMethod;
  }

  private static volatile io.grpc.MethodDescriptor<org.apache.arrow.flight.impl.Flight.FlightData,
      org.apache.arrow.flight.impl.Flight.PutResult> getDoPutMethod;

  @io.grpc.stub.annotations.RpcMethod(
      fullMethodName = SERVICE_NAME + '/' + "DoPut",
      requestType = org.apache.arrow.flight.impl.Flight.FlightData.class,
      responseType = org.apache.arrow.flight.impl.Flight.PutResult.class,
      methodType = io.grpc.MethodDescriptor.MethodType.BIDI_STREAMING)
  public static io.grpc.MethodDescriptor<org.apache.arrow.flight.impl.Flight.FlightData,
      org.apache.arrow.flight.impl.Flight.PutResult> getDoPutMethod() {
    io.grpc.MethodDescriptor<org.apache.arrow.flight.impl.Flight.FlightData, org.apache.arrow.flight.impl.Flight.PutResult> getDoPutMethod;
    if ((getDoPutMethod = FlightServiceGrpc.getDoPutMethod) == null) {
      synchronized (FlightServiceGrpc.class) {
        if ((getDoPutMethod = FlightServiceGrpc.getDoPutMethod) == null) {
          FlightServiceGrpc.getDoPutMethod = getDoPutMethod =
              io.grpc.MethodDescriptor.<org.apache.arrow.flight.impl.Flight.FlightData, org.apache.arrow.flight.impl.Flight.PutResult>newBuilder()
              .setType(io.grpc.MethodDescriptor.MethodType.BIDI_STREAMING)
              .setFullMethodName(generateFullMethodName(SERVICE_NAME, "DoPut"))
              .setSampledToLocalTracing(true)
              .setRequestMarshaller(io.grpc.protobuf.ProtoUtils.marshaller(
                  org.apache.arrow.flight.impl.Flight.FlightData.getDefaultInstance()))
              .setResponseMarshaller(io.grpc.protobuf.ProtoUtils.marshaller(
                  org.apache.arrow.flight.impl.Flight.PutResult.getDefaultInstance()))
              .setSchemaDescriptor(new FlightServiceMethodDescriptorSupplier("DoPut"))
              .build();
        }
      }
    }
    return getDoPutMethod;
  }

  private static volatile io.grpc.MethodDescriptor<org.apache.arrow.flight.impl.Flight.FlightData,
      org.apache.arrow.flight.impl.Flight.FlightData> getDoExchangeMethod;

  @io.grpc.stub.annotations.RpcMethod(
      fullMethodName = SERVICE_NAME + '/' + "DoExchange",
      requestType = org.apache.arrow.flight.impl.Flight.FlightData.class,
      responseType = org.apache.arrow.flight.impl.Flight.FlightData.class,
      methodType = io.grpc.MethodDescriptor.MethodType.BIDI_STREAMING)
  public static io.grpc.MethodDescriptor<org.apache.arrow.flight.impl.Flight.FlightData,
      org.apache.arrow.flight.impl.Flight.FlightData> getDoExchangeMethod() {
    io.grpc.MethodDescriptor<org.apache.arrow.flight.impl.Flight.FlightData, org.apache.arrow.flight.impl.Flight.FlightData> getDoExchangeMethod;
    if ((getDoExchangeMethod = FlightServiceGrpc.getDoExchangeMethod) == null) {
      synchronized (FlightServiceGrpc.class) {
        if ((getDoExchangeMethod = FlightServiceGrpc.getDoExchangeMethod) == null) {
          FlightServiceGrpc.getDoExchangeMethod = getDoExchangeMethod =
              io.grpc.MethodDescriptor.<org.apache.arrow.flight.impl.Flight.FlightData, org.apache.arrow.flight.impl.Flight.FlightData>newBuilder()
              .setType(io.grpc.MethodDescriptor.MethodType.BIDI_STREAMING)
              .setFullMethodName(generateFullMethodName(SERVICE_NAME, "DoExchange"))
              .setSampledToLocalTracing(true)
              .setRequestMarshaller(io.grpc.protobuf.ProtoUtils.marshaller(
                  org.apache.arrow.flight.impl.Flight.FlightData.getDefaultInstance()))
              .setResponseMarshaller(io.grpc.protobuf.ProtoUtils.marshaller(
                  org.apache.arrow.flight.impl.Flight.FlightData.getDefaultInstance()))
              .setSchemaDescriptor(new FlightServiceMethodDescriptorSupplier("DoExchange"))
              .build();
        }
      }
    }
    return getDoExchangeMethod;
  }

  private static volatile io.grpc.MethodDescriptor<org.apache.arrow.flight.impl.Flight.Action,
      org.apache.arrow.flight.impl.Flight.Result> getDoActionMethod;

  @io.grpc.stub.annotations.RpcMethod(
      fullMethodName = SERVICE_NAME + '/' + "DoAction",
      requestType = org.apache.arrow.flight.impl.Flight.Action.class,
      responseType = org.apache.arrow.flight.impl.Flight.Result.class,
      methodType = io.grpc.MethodDescriptor.MethodType.SERVER_STREAMING)
  public static io.grpc.MethodDescriptor<org.apache.arrow.flight.impl.Flight.Action,
      org.apache.arrow.flight.impl.Flight.Result> getDoActionMethod() {
    io.grpc.MethodDescriptor<org.apache.arrow.flight.impl.Flight.Action, org.apache.arrow.flight.impl.Flight.Result> getDoActionMethod;
    if ((getDoActionMethod = FlightServiceGrpc.getDoActionMethod) == null) {
      synchronized (FlightServiceGrpc.class) {
        if ((getDoActionMethod = FlightServiceGrpc.getDoActionMethod) == null) {
          FlightServiceGrpc.getDoActionMethod = getDoActionMethod =
              io.grpc.MethodDescriptor.<org.apache.arrow.flight.impl.Flight.Action, org.apache.arrow.flight.impl.Flight.Result>newBuilder()
              .setType(io.grpc.MethodDescriptor.MethodType.SERVER_STREAMING)
              .setFullMethodName(generateFullMethodName(SERVICE_NAME, "DoAction"))
              .setSampledToLocalTracing(true)
              .setRequestMarshaller(io.grpc.protobuf.ProtoUtils.marshaller(
                  org.apache.arrow.flight.impl.Flight.Action.getDefaultInstance()))
              .setResponseMarshaller(io.grpc.protobuf.ProtoUtils.marshaller(
                  org.apache.arrow.flight.impl.Flight.Result.getDefaultInstance()))
              .setSchemaDescriptor(new FlightServiceMethodDescriptorSupplier("DoAction"))
              .build();
        }
      }
    }
    return getDoActionMethod;
  }

  private static volatile io.grpc.MethodDescriptor<org.apache.arrow.flight.impl.Flight.Empty,
      org.apache.arrow.flight.impl.Flight.ActionType> getListActionsMethod;

  @io.grpc.stub.annotations.RpcMethod(
      fullMethodName = SERVICE_NAME + '/' + "ListActions",
      requestType = org.apache.arrow.flight.impl.Flight.Empty.class,
      responseType = org.apache.arrow.flight.impl.Flight.ActionType.class,
      methodType = io.grpc.MethodDescriptor.MethodType.SERVER_STREAMING)
  public static io.grpc.MethodDescriptor<org.apache.arrow.flight.impl.Flight.Empty,
      org.apache.arrow.flight.impl.Flight.ActionType> getListActionsMethod() {
    io.grpc.MethodDescriptor<org.apache.arrow.flight.impl.Flight.Empty, org.apache.arrow.flight.impl.Flight.ActionType> getListActionsMethod;
    if ((getListActionsMethod = FlightServiceGrpc.getListActionsMethod) == null) {
      synchronized (FlightServiceGrpc.class) {
        if ((getListActionsMethod = FlightServiceGrpc.getListActionsMethod) == null) {
          FlightServiceGrpc.getListActionsMethod = getListActionsMethod =
              io.grpc.MethodDescriptor.<org.apache.arrow.flight.impl.Flight.Empty, org.apache.arrow.flight.impl.Flight.ActionType>newBuilder()
              .setType(io.grpc.MethodDescriptor.MethodType.SERVER_STREAMING)
              .setFullMethodName(generateFullMethodName(SERVICE_NAME, "ListActions"))
              .setSampledToLocalTracing(true)
              .setRequestMarshaller(io.grpc.protobuf.ProtoUtils.marshaller(
                  org.apache.arrow.flight.impl.Flight.Empty.getDefaultInstance()))
              .setResponseMarshaller(io.grpc.protobuf.ProtoUtils.marshaller(
                  org.apache.arrow.flight.impl.Flight.ActionType.getDefaultInstance()))
              .setSchemaDescriptor(new FlightServiceMethodDescriptorSupplier("ListActions"))
              .build();
        }
      }
    }
    return getListActionsMethod;
  }

  /**
   * Creates a new async stub that supports all call types for the service
   */
  public static FlightServiceStub newStub(io.grpc.Channel channel) {
    io.grpc.stub.AbstractStub.StubFactory<FlightServiceStub> factory =
      new io.grpc.stub.AbstractStub.StubFactory<FlightServiceStub>() {
        @java.lang.Override
        public FlightServiceStub newStub(io.grpc.Channel channel, io.grpc.CallOptions callOptions) {
          return new FlightServiceStub(channel, callOptions);
        }
      };
    return FlightServiceStub.newStub(factory, channel);
  }

  /**
   * Creates a new blocking-style stub that supports unary and streaming output calls on the service
   */
  public static FlightServiceBlockingStub newBlockingStub(
      io.grpc.Channel channel) {
    io.grpc.stub.AbstractStub.StubFactory<FlightServiceBlockingStub> factory =
      new io.grpc.stub.AbstractStub.StubFactory<FlightServiceBlockingStub>() {
        @java.lang.Override
        public FlightServiceBlockingStub newStub(io.grpc.Channel channel, io.grpc.CallOptions callOptions) {
          return new FlightServiceBlockingStub(channel, callOptions);
        }
      };
    return FlightServiceBlockingStub.newStub(factory, channel);
  }

  /**
   * Creates a new ListenableFuture-style stub that supports unary calls on the service
   */
  public static FlightServiceFutureStub newFutureStub(
      io.grpc.Channel channel) {
    io.grpc.stub.AbstractStub.StubFactory<FlightServiceFutureStub> factory =
      new io.grpc.stub.AbstractStub.StubFactory<FlightServiceFutureStub>() {
        @java.lang.Override
        public FlightServiceFutureStub newStub(io.grpc.Channel channel, io.grpc.CallOptions callOptions) {
          return new FlightServiceFutureStub(channel, callOptions);
        }
      };
    return FlightServiceFutureStub.newStub(factory, channel);
  }

  /**
   * <pre>
   * A flight service is an endpoint for retrieving or storing Arrow data. A
   * flight service can expose one or more predefined endpoints that can be
   * accessed using the Arrow Flight Protocol. Additionally, a flight service
   * can expose a set of actions that are available.
   * </pre>
   */
  public interface AsyncService {

    /**
     * <pre>
     * Handshake between client and server. Depending on the server, the
     * handshake may be required to determine the token that should be used for
     * future operations. Both request and response are streams to allow multiple
     * round-trips depending on auth mechanism.
     * </pre>
     */
    default io.grpc.stub.StreamObserver<org.apache.arrow.flight.impl.Flight.HandshakeRequest> handshake(
        io.grpc.stub.StreamObserver<org.apache.arrow.flight.impl.Flight.HandshakeResponse> responseObserver) {
      return io.grpc.stub.ServerCalls.asyncUnimplementedStreamingCall(getHandshakeMethod(), responseObserver);
    }

    /**
     * <pre>
     * Get a list of available streams given a particular criteria. Most flight
     * services will expose one or more streams that are readily available for
     * retrieval. This api allows listing the streams available for
     * consumption. A user can also provide a criteria. The criteria can limit
     * the subset of streams that can be listed via this interface. Each flight
     * service allows its own definition of how to consume criteria.
     * </pre>
     */
    default void listFlights(org.apache.arrow.flight.impl.Flight.Criteria request,
        io.grpc.stub.StreamObserver<org.apache.arrow.flight.impl.Flight.FlightInfo> responseObserver) {
      io.grpc.stub.ServerCalls.asyncUnimplementedUnaryCall(getListFlightsMethod(), responseObserver);
    }

    /**
     * <pre>
     * For a given FlightDescriptor, get information about how the flight can be
     * consumed. This is a useful interface if the consumer of the interface
     * already can identify the specific flight to consume. This interface can
     * also allow a consumer to generate a flight stream through a specified
     * descriptor. For example, a flight descriptor might be something that
     * includes a SQL statement or a Pickled Python operation that will be
     * executed. In those cases, the descriptor will not be previously available
     * within the list of available streams provided by ListFlights but will be
     * available for consumption for the duration defined by the specific flight
     * service.
     * </pre>
     */
    default void getFlightInfo(org.apache.arrow.flight.impl.Flight.FlightDescriptor request,
        io.grpc.stub.StreamObserver<org.apache.arrow.flight.impl.Flight.FlightInfo> responseObserver) {
      io.grpc.stub.ServerCalls.asyncUnimplementedUnaryCall(getGetFlightInfoMethod(), responseObserver);
    }

    /**
     * <pre>
     * For a given FlightDescriptor, start a query and get information
     * to poll its execution status. This is a useful interface if the
     * query may be a long-running query. The first PollFlightInfo call
     * should return as quickly as possible. (GetFlightInfo doesn't
     * return until the query is complete.)
     * A client can consume any available results before
     * the query is completed. See PollInfo.info for details.
     * A client can poll the updated query status by calling
     * PollFlightInfo() with PollInfo.flight_descriptor. A server
     * should not respond until the result would be different from last
     * time. That way, the client can "long poll" for updates
     * without constantly making requests. Clients can set a short timeout
     * to avoid blocking calls if desired.
     * A client can't use PollInfo.flight_descriptor after
     * PollInfo.expiration_time passes. A server might not accept the
     * retry descriptor anymore and the query may be cancelled.
     * A client may use the CancelFlightInfo action with
     * PollInfo.info to cancel the running query.
     * </pre>
     */
    default void pollFlightInfo(org.apache.arrow.flight.impl.Flight.FlightDescriptor request,
        io.grpc.stub.StreamObserver<org.apache.arrow.flight.impl.Flight.PollInfo> responseObserver) {
      io.grpc.stub.ServerCalls.asyncUnimplementedUnaryCall(getPollFlightInfoMethod(), responseObserver);
    }

    /**
     * <pre>
     * For a given FlightDescriptor, get the Schema as described in Schema.fbs::Schema
     * This is used when a consumer needs the Schema of flight stream. Similar to
     * GetFlightInfo this interface may generate a new flight that was not previously
     * available in ListFlights.
     * </pre>
     */
    default void getSchema(org.apache.arrow.flight.impl.Flight.FlightDescriptor request,
        io.grpc.stub.StreamObserver<org.apache.arrow.flight.impl.Flight.SchemaResult> responseObserver) {
      io.grpc.stub.ServerCalls.asyncUnimplementedUnaryCall(getGetSchemaMethod(), responseObserver);
    }

    /**
     * <pre>
     * Retrieve a single stream associated with a particular descriptor
     * associated with the referenced ticket. A Flight can be composed of one or
     * more streams where each stream can be retrieved using a separate opaque
     * ticket that the flight service uses for managing a collection of streams.
     * </pre>
     */
    default void doGet(org.apache.arrow.flight.impl.Flight.Ticket request,
        io.grpc.stub.StreamObserver<org.apache.arrow.flight.impl.Flight.FlightData> responseObserver) {
      io.grpc.stub.ServerCalls.asyncUnimplementedUnaryCall(getDoGetMethod(), responseObserver);
    }

    /**
     * <pre>
     * Push a stream to the flight service associated with a particular
     * flight stream. This allows a client of a flight service to upload a stream
     * of data. Depending on the particular flight service, a client consumer
     * could be allowed to upload a single stream per descriptor or an unlimited
     * number. In the latter, the service might implement a 'seal' action that
     * can be applied to a descriptor once all streams are uploaded.
     * </pre>
     */
    default io.grpc.stub.StreamObserver<org.apache.arrow.flight.impl.Flight.FlightData> doPut(
        io.grpc.stub.StreamObserver<org.apache.arrow.flight.impl.Flight.PutResult> responseObserver) {
      return io.grpc.stub.ServerCalls.asyncUnimplementedStreamingCall(getDoPutMethod(), responseObserver);
    }

    /**
     * <pre>
     * Open a bidirectional data channel for a given descriptor. This
     * allows clients to send and receive arbitrary Arrow data and
     * application-specific metadata in a single logical stream. In
     * contrast to DoGet/DoPut, this is more suited for clients
     * offloading computation (rather than storage) to a Flight service.
     * </pre>
     */
    default io.grpc.stub.StreamObserver<org.apache.arrow.flight.impl.Flight.FlightData> doExchange(
        io.grpc.stub.StreamObserver<org.apache.arrow.flight.impl.Flight.FlightData> responseObserver) {
      return io.grpc.stub.ServerCalls.asyncUnimplementedStreamingCall(getDoExchangeMethod(), responseObserver);
    }

    /**
     * <pre>
     * Flight services can support an arbitrary number of simple actions in
     * addition to the possible ListFlights, GetFlightInfo, DoGet, DoPut
     * operations that are potentially available. DoAction allows a flight client
     * to do a specific action against a flight service. An action includes
     * opaque request and response objects that are specific to the type action
     * being undertaken.
     * </pre>
     */
    default void doAction(org.apache.arrow.flight.impl.Flight.Action request,
        io.grpc.stub.StreamObserver<org.apache.arrow.flight.impl.Flight.Result> responseObserver) {
      io.grpc.stub.ServerCalls.asyncUnimplementedUnaryCall(getDoActionMethod(), responseObserver);
    }

    /**
     * <pre>
     * A flight service exposes all of the available action types that it has
     * along with descriptions. This allows different flight consumers to
     * understand the capabilities of the flight service.
     * </pre>
     */
    default void listActions(org.apache.arrow.flight.impl.Flight.Empty request,
        io.grpc.stub.StreamObserver<org.apache.arrow.flight.impl.Flight.ActionType> responseObserver) {
      io.grpc.stub.ServerCalls.asyncUnimplementedUnaryCall(getListActionsMethod(), responseObserver);
    }
  }

  /**
   * Base class for the server implementation of the service FlightService.
   * <pre>
   * A flight service is an endpoint for retrieving or storing Arrow data. A
   * flight service can expose one or more predefined endpoints that can be
   * accessed using the Arrow Flight Protocol. Additionally, a flight service
   * can expose a set of actions that are available.
   * </pre>
   */
  public static abstract class FlightServiceImplBase
      implements io.grpc.BindableService, AsyncService {

    @java.lang.Override public final io.grpc.ServerServiceDefinition bindService() {
      return FlightServiceGrpc.bindService(this);
    }
  }

  /**
   * A stub to allow clients to do asynchronous rpc calls to service FlightService.
   * <pre>
   * A flight service is an endpoint for retrieving or storing Arrow data. A
   * flight service can expose one or more predefined endpoints that can be
   * accessed using the Arrow Flight Protocol. Additionally, a flight service
   * can expose a set of actions that are available.
   * </pre>
   */
  public static final class FlightServiceStub
      extends io.grpc.stub.AbstractAsyncStub<FlightServiceStub> {
    private FlightServiceStub(
        io.grpc.Channel channel, io.grpc.CallOptions callOptions) {
      super(channel, callOptions);
    }

    @java.lang.Override
    protected FlightServiceStub build(
        io.grpc.Channel channel, io.grpc.CallOptions callOptions) {
      return new FlightServiceStub(channel, callOptions);
    }

    /**
     * <pre>
     * Handshake between client and server. Depending on the server, the
     * handshake may be required to determine the token that should be used for
     * future operations. Both request and response are streams to allow multiple
     * round-trips depending on auth mechanism.
     * </pre>
     */
    public io.grpc.stub.StreamObserver<org.apache.arrow.flight.impl.Flight.HandshakeRequest> handshake(
        io.grpc.stub.StreamObserver<org.apache.arrow.flight.impl.Flight.HandshakeResponse> responseObserver) {
      return io.grpc.stub.ClientCalls.asyncBidiStreamingCall(
          getChannel().newCall(getHandshakeMethod(), getCallOptions()), responseObserver);
    }

    /**
     * <pre>
     * Get a list of available streams given a particular criteria. Most flight
     * services will expose one or more streams that are readily available for
     * retrieval. This api allows listing the streams available for
     * consumption. A user can also provide a criteria. The criteria can limit
     * the subset of streams that can be listed via this interface. Each flight
     * service allows its own definition of how to consume criteria.
     * </pre>
     */
    public void listFlights(org.apache.arrow.flight.impl.Flight.Criteria request,
        io.grpc.stub.StreamObserver<org.apache.arrow.flight.impl.Flight.FlightInfo> responseObserver) {
      io.grpc.stub.ClientCalls.asyncServerStreamingCall(
          getChannel().newCall(getListFlightsMethod(), getCallOptions()), request, responseObserver);
    }

    /**
     * <pre>
     * For a given FlightDescriptor, get information about how the flight can be
     * consumed. This is a useful interface if the consumer of the interface
     * already can identify the specific flight to consume. This interface can
     * also allow a consumer to generate a flight stream through a specified
     * descriptor. For example, a flight descriptor might be something that
     * includes a SQL statement or a Pickled Python operation that will be
     * executed. In those cases, the descriptor will not be previously available
     * within the list of available streams provided by ListFlights but will be
     * available for consumption for the duration defined by the specific flight
     * service.
     * </pre>
     */
    public void getFlightInfo(org.apache.arrow.flight.impl.Flight.FlightDescriptor request,
        io.grpc.stub.StreamObserver<org.apache.arrow.flight.impl.Flight.FlightInfo> responseObserver) {
      io.grpc.stub.ClientCalls.asyncUnaryCall(
          getChannel().newCall(getGetFlightInfoMethod(), getCallOptions()), request, responseObserver);
    }

    /**
     * <pre>
     * For a given FlightDescriptor, start a query and get information
     * to poll its execution status. This is a useful interface if the
     * query may be a long-running query. The first PollFlightInfo call
     * should return as quickly as possible. (GetFlightInfo doesn't
     * return until the query is complete.)
     * A client can consume any available results before
     * the query is completed. See PollInfo.info for details.
     * A client can poll the updated query status by calling
     * PollFlightInfo() with PollInfo.flight_descriptor. A server
     * should not respond until the result would be different from last
     * time. That way, the client can "long poll" for updates
     * without constantly making requests. Clients can set a short timeout
     * to avoid blocking calls if desired.
     * A client can't use PollInfo.flight_descriptor after
     * PollInfo.expiration_time passes. A server might not accept the
     * retry descriptor anymore and the query may be cancelled.
     * A client may use the CancelFlightInfo action with
     * PollInfo.info to cancel the running query.
     * </pre>
     */
    public void pollFlightInfo(org.apache.arrow.flight.impl.Flight.FlightDescriptor request,
        io.grpc.stub.StreamObserver<org.apache.arrow.flight.impl.Flight.PollInfo> responseObserver) {
      io.grpc.stub.ClientCalls.asyncUnaryCall(
          getChannel().newCall(getPollFlightInfoMethod(), getCallOptions()), request, responseObserver);
    }

    /**
     * <pre>
     * For a given FlightDescriptor, get the Schema as described in Schema.fbs::Schema
     * This is used when a consumer needs the Schema of flight stream. Similar to
     * GetFlightInfo this interface may generate a new flight that was not previously
     * available in ListFlights.
     * </pre>
     */
    public void getSchema(org.apache.arrow.flight.impl.Flight.FlightDescriptor request,
        io.grpc.stub.StreamObserver<org.apache.arrow.flight.impl.Flight.SchemaResult> responseObserver) {
      io.grpc.stub.ClientCalls.asyncUnaryCall(
          getChannel().newCall(getGetSchemaMethod(), getCallOptions()), request, responseObserver);
    }

    /**
     * <pre>
     * Retrieve a single stream associated with a particular descriptor
     * associated with the referenced ticket. A Flight can be composed of one or
     * more streams where each stream can be retrieved using a separate opaque
     * ticket that the flight service uses for managing a collection of streams.
     * </pre>
     */
    public void doGet(org.apache.arrow.flight.impl.Flight.Ticket request,
        io.grpc.stub.StreamObserver<org.apache.arrow.flight.impl.Flight.FlightData> responseObserver) {
      io.grpc.stub.ClientCalls.asyncServerStreamingCall(
          getChannel().newCall(getDoGetMethod(), getCallOptions()), request, responseObserver);
    }

    /**
     * <pre>
     * Push a stream to the flight service associated with a particular
     * flight stream. This allows a client of a flight service to upload a stream
     * of data. Depending on the particular flight service, a client consumer
     * could be allowed to upload a single stream per descriptor or an unlimited
     * number. In the latter, the service might implement a 'seal' action that
     * can be applied to a descriptor once all streams are uploaded.
     * </pre>
     */
    public io.grpc.stub.StreamObserver<org.apache.arrow.flight.impl.Flight.FlightData> doPut(
        io.grpc.stub.StreamObserver<org.apache.arrow.flight.impl.Flight.PutResult> responseObserver) {
      return io.grpc.stub.ClientCalls.asyncBidiStreamingCall(
          getChannel().newCall(getDoPutMethod(), getCallOptions()), responseObserver);
    }

    /**
     * <pre>
     * Open a bidirectional data channel for a given descriptor. This
     * allows clients to send and receive arbitrary Arrow data and
     * application-specific metadata in a single logical stream. In
     * contrast to DoGet/DoPut, this is more suited for clients
     * offloading computation (rather than storage) to a Flight service.
     * </pre>
     */
    public io.grpc.stub.StreamObserver<org.apache.arrow.flight.impl.Flight.FlightData> doExchange(
        io.grpc.stub.StreamObserver<org.apache.arrow.flight.impl.Flight.FlightData> responseObserver) {
      return io.grpc.stub.ClientCalls.asyncBidiStreamingCall(
          getChannel().newCall(getDoExchangeMethod(), getCallOptions()), responseObserver);
    }

    /**
     * <pre>
     * Flight services can support an arbitrary number of simple actions in
     * addition to the possible ListFlights, GetFlightInfo, DoGet, DoPut
     * operations that are potentially available. DoAction allows a flight client
     * to do a specific action against a flight service. An action includes
     * opaque request and response objects that are specific to the type action
     * being undertaken.
     * </pre>
     */
    public void doAction(org.apache.arrow.flight.impl.Flight.Action request,
        io.grpc.stub.StreamObserver<org.apache.arrow.flight.impl.Flight.Result> responseObserver) {
      io.grpc.stub.ClientCalls.asyncServerStreamingCall(
          getChannel().newCall(getDoActionMethod(), getCallOptions()), request, responseObserver);
    }

    /**
     * <pre>
     * A flight service exposes all of the available action types that it has
     * along with descriptions. This allows different flight consumers to
     * understand the capabilities of the flight service.
     * </pre>
     */
    public void listActions(org.apache.arrow.flight.impl.Flight.Empty request,
        io.grpc.stub.StreamObserver<org.apache.arrow.flight.impl.Flight.ActionType> responseObserver) {
      io.grpc.stub.ClientCalls.asyncServerStreamingCall(
          getChannel().newCall(getListActionsMethod(), getCallOptions()), request, responseObserver);
    }
  }

  /**
   * A stub to allow clients to do synchronous rpc calls to service FlightService.
   * <pre>
   * A flight service is an endpoint for retrieving or storing Arrow data. A
   * flight service can expose one or more predefined endpoints that can be
   * accessed using the Arrow Flight Protocol. Additionally, a flight service
   * can expose a set of actions that are available.
   * </pre>
   */
  public static final class FlightServiceBlockingStub
      extends io.grpc.stub.AbstractBlockingStub<FlightServiceBlockingStub> {
    private FlightServiceBlockingStub(
        io.grpc.Channel channel, io.grpc.CallOptions callOptions) {
      super(channel, callOptions);
    }

    @java.lang.Override
    protected FlightServiceBlockingStub build(
        io.grpc.Channel channel, io.grpc.CallOptions callOptions) {
      return new FlightServiceBlockingStub(channel, callOptions);
    }

    /**
     * <pre>
     * Get a list of available streams given a particular criteria. Most flight
     * services will expose one or more streams that are readily available for
     * retrieval. This api allows listing the streams available for
     * consumption. A user can also provide a criteria. The criteria can limit
     * the subset of streams that can be listed via this interface. Each flight
     * service allows its own definition of how to consume criteria.
     * </pre>
     */
    public java.util.Iterator<org.apache.arrow.flight.impl.Flight.FlightInfo> listFlights(
        org.apache.arrow.flight.impl.Flight.Criteria request) {
      return io.grpc.stub.ClientCalls.blockingServerStreamingCall(
          getChannel(), getListFlightsMethod(), getCallOptions(), request);
    }

    /**
     * <pre>
     * For a given FlightDescriptor, get information about how the flight can be
     * consumed. This is a useful interface if the consumer of the interface
     * already can identify the specific flight to consume. This interface can
     * also allow a consumer to generate a flight stream through a specified
     * descriptor. For example, a flight descriptor might be something that
     * includes a SQL statement or a Pickled Python operation that will be
     * executed. In those cases, the descriptor will not be previously available
     * within the list of available streams provided by ListFlights but will be
     * available for consumption for the duration defined by the specific flight
     * service.
     * </pre>
     */
    public org.apache.arrow.flight.impl.Flight.FlightInfo getFlightInfo(org.apache.arrow.flight.impl.Flight.FlightDescriptor request) {
      return io.grpc.stub.ClientCalls.blockingUnaryCall(
          getChannel(), getGetFlightInfoMethod(), getCallOptions(), request);
    }

    /**
     * <pre>
     * For a given FlightDescriptor, start a query and get information
     * to poll its execution status. This is a useful interface if the
     * query may be a long-running query. The first PollFlightInfo call
     * should return as quickly as possible. (GetFlightInfo doesn't
     * return until the query is complete.)
     * A client can consume any available results before
     * the query is completed. See PollInfo.info for details.
     * A client can poll the updated query status by calling
     * PollFlightInfo() with PollInfo.flight_descriptor. A server
     * should not respond until the result would be different from last
     * time. That way, the client can "long poll" for updates
     * without constantly making requests. Clients can set a short timeout
     * to avoid blocking calls if desired.
     * A client can't use PollInfo.flight_descriptor after
     * PollInfo.expiration_time passes. A server might not accept the
     * retry descriptor anymore and the query may be cancelled.
     * A client may use the CancelFlightInfo action with
     * PollInfo.info to cancel the running query.
     * </pre>
     */
    public org.apache.arrow.flight.impl.Flight.PollInfo pollFlightInfo(org.apache.arrow.flight.impl.Flight.FlightDescriptor request) {
      return io.grpc.stub.ClientCalls.blockingUnaryCall(
          getChannel(), getPollFlightInfoMethod(), getCallOptions(), request);
    }

    /**
     * <pre>
     * For a given FlightDescriptor, get the Schema as described in Schema.fbs::Schema
     * This is used when a consumer needs the Schema of flight stream. Similar to
     * GetFlightInfo this interface may generate a new flight that was not previously
     * available in ListFlights.
     * </pre>
     */
    public org.apache.arrow.flight.impl.Flight.SchemaResult getSchema(org.apache.arrow.flight.impl.Flight.FlightDescriptor request) {
      return io.grpc.stub.ClientCalls.blockingUnaryCall(
          getChannel(), getGetSchemaMethod(), getCallOptions(), request);
    }

    /**
     * <pre>
     * Retrieve a single stream associated with a particular descriptor
     * associated with the referenced ticket. A Flight can be composed of one or
     * more streams where each stream can be retrieved using a separate opaque
     * ticket that the flight service uses for managing a collection of streams.
     * </pre>
     */
    public java.util.Iterator<org.apache.arrow.flight.impl.Flight.FlightData> doGet(
        org.apache.arrow.flight.impl.Flight.Ticket request) {
      return io.grpc.stub.ClientCalls.blockingServerStreamingCall(
          getChannel(), getDoGetMethod(), getCallOptions(), request);
    }

    /**
     * <pre>
     * Flight services can support an arbitrary number of simple actions in
     * addition to the possible ListFlights, GetFlightInfo, DoGet, DoPut
     * operations that are potentially available. DoAction allows a flight client
     * to do a specific action against a flight service. An action includes
     * opaque request and response objects that are specific to the type action
     * being undertaken.
     * </pre>
     */
    public java.util.Iterator<org.apache.arrow.flight.impl.Flight.Result> doAction(
        org.apache.arrow.flight.impl.Flight.Action request) {
      return io.grpc.stub.ClientCalls.blockingServerStreamingCall(
          getChannel(), getDoActionMethod(), getCallOptions(), request);
    }

    /**
     * <pre>
     * A flight service exposes all of the available action types that it has
     * along with descriptions. This allows different flight consumers to
     * understand the capabilities of the flight service.
     * </pre>
     */
    public java.util.Iterator<org.apache.arrow.flight.impl.Flight.ActionType> listActions(
        org.apache.arrow.flight.impl.Flight.Empty request) {
      return io.grpc.stub.ClientCalls.blockingServerStreamingCall(
          getChannel(), getListActionsMethod(), getCallOptions(), request);
    }
  }

  /**
   * A stub to allow clients to do ListenableFuture-style rpc calls to service FlightService.
   * <pre>
   * A flight service is an endpoint for retrieving or storing Arrow data. A
   * flight service can expose one or more predefined endpoints that can be
   * accessed using the Arrow Flight Protocol. Additionally, a flight service
   * can expose a set of actions that are available.
   * </pre>
   */
  public static final class FlightServiceFutureStub
      extends io.grpc.stub.AbstractFutureStub<FlightServiceFutureStub> {
    private FlightServiceFutureStub(
        io.grpc.Channel channel, io.grpc.CallOptions callOptions) {
      super(channel, callOptions);
    }

    @java.lang.Override
    protected FlightServiceFutureStub build(
        io.grpc.Channel channel, io.grpc.CallOptions callOptions) {
      return new FlightServiceFutureStub(channel, callOptions);
    }

    /**
     * <pre>
     * For a given FlightDescriptor, get information about how the flight can be
     * consumed. This is a useful interface if the consumer of the interface
     * already can identify the specific flight to consume. This interface can
     * also allow a consumer to generate a flight stream through a specified
     * descriptor. For example, a flight descriptor might be something that
     * includes a SQL statement or a Pickled Python operation that will be
     * executed. In those cases, the descriptor will not be previously available
     * within the list of available streams provided by ListFlights but will be
     * available for consumption for the duration defined by the specific flight
     * service.
     * </pre>
     */
    public com.google.common.util.concurrent.ListenableFuture<org.apache.arrow.flight.impl.Flight.FlightInfo> getFlightInfo(
        org.apache.arrow.flight.impl.Flight.FlightDescriptor request) {
      return io.grpc.stub.ClientCalls.futureUnaryCall(
          getChannel().newCall(getGetFlightInfoMethod(), getCallOptions()), request);
    }

    /**
     * <pre>
     * For a given FlightDescriptor, start a query and get information
     * to poll its execution status. This is a useful interface if the
     * query may be a long-running query. The first PollFlightInfo call
     * should return as quickly as possible. (GetFlightInfo doesn't
     * return until the query is complete.)
     * A client can consume any available results before
     * the query is completed. See PollInfo.info for details.
     * A client can poll the updated query status by calling
     * PollFlightInfo() with PollInfo.flight_descriptor. A server
     * should not respond until the result would be different from last
     * time. That way, the client can "long poll" for updates
     * without constantly making requests. Clients can set a short timeout
     * to avoid blocking calls if desired.
     * A client can't use PollInfo.flight_descriptor after
     * PollInfo.expiration_time passes. A server might not accept the
     * retry descriptor anymore and the query may be cancelled.
     * A client may use the CancelFlightInfo action with
     * PollInfo.info to cancel the running query.
     * </pre>
     */
    public com.google.common.util.concurrent.ListenableFuture<org.apache.arrow.flight.impl.Flight.PollInfo> pollFlightInfo(
        org.apache.arrow.flight.impl.Flight.FlightDescriptor request) {
      return io.grpc.stub.ClientCalls.futureUnaryCall(
          getChannel().newCall(getPollFlightInfoMethod(), getCallOptions()), request);
    }

    /**
     * <pre>
     * For a given FlightDescriptor, get the Schema as described in Schema.fbs::Schema
     * This is used when a consumer needs the Schema of flight stream. Similar to
     * GetFlightInfo this interface may generate a new flight that was not previously
     * available in ListFlights.
     * </pre>
     */
    public com.google.common.util.concurrent.ListenableFuture<org.apache.arrow.flight.impl.Flight.SchemaResult> getSchema(
        org.apache.arrow.flight.impl.Flight.FlightDescriptor request) {
      return io.grpc.stub.ClientCalls.futureUnaryCall(
          getChannel().newCall(getGetSchemaMethod(), getCallOptions()), request);
    }
  }

  private static final int METHODID_LIST_FLIGHTS = 0;
  private static final int METHODID_GET_FLIGHT_INFO = 1;
  private static final int METHODID_POLL_FLIGHT_INFO = 2;
  private static final int METHODID_GET_SCHEMA = 3;
  private static final int METHODID_DO_GET = 4;
  private static final int METHODID_DO_ACTION = 5;
  private static final int METHODID_LIST_ACTIONS = 6;
  private static final int METHODID_HANDSHAKE = 7;
  private static final int METHODID_DO_PUT = 8;
  private static final int METHODID_DO_EXCHANGE = 9;

  private static final class MethodHandlers<Req, Resp> implements
      io.grpc.stub.ServerCalls.UnaryMethod<Req, Resp>,
      io.grpc.stub.ServerCalls.ServerStreamingMethod<Req, Resp>,
      io.grpc.stub.ServerCalls.ClientStreamingMethod<Req, Resp>,
      io.grpc.stub.ServerCalls.BidiStreamingMethod<Req, Resp> {
    private final AsyncService serviceImpl;
    private final int methodId;

    MethodHandlers(AsyncService serviceImpl, int methodId) {
      this.serviceImpl = serviceImpl;
      this.methodId = methodId;
    }

    @java.lang.Override
    @java.lang.SuppressWarnings("unchecked")
    public void invoke(Req request, io.grpc.stub.StreamObserver<Resp> responseObserver) {
      switch (methodId) {
        case METHODID_LIST_FLIGHTS:
          serviceImpl.listFlights((org.apache.arrow.flight.impl.Flight.Criteria) request,
              (io.grpc.stub.StreamObserver<org.apache.arrow.flight.impl.Flight.FlightInfo>) responseObserver);
          break;
        case METHODID_GET_FLIGHT_INFO:
          serviceImpl.getFlightInfo((org.apache.arrow.flight.impl.Flight.FlightDescriptor) request,
              (io.grpc.stub.StreamObserver<org.apache.arrow.flight.impl.Flight.FlightInfo>) responseObserver);
          break;
        case METHODID_POLL_FLIGHT_INFO:
          serviceImpl.pollFlightInfo((org.apache.arrow.flight.impl.Flight.FlightDescriptor) request,
              (io.grpc.stub.StreamObserver<org.apache.arrow.flight.impl.Flight.PollInfo>) responseObserver);
          break;
        case METHODID_GET_SCHEMA:
          serviceImpl.getSchema((org.apache.arrow.flight.impl.Flight.FlightDescriptor) request,
              (io.grpc.stub.StreamObserver<org.apache.arrow.flight.impl.Flight.SchemaResult>) responseObserver);
          break;
        case METHODID_DO_GET:
          serviceImpl.doGet((org.apache.arrow.flight.impl.Flight.Ticket) request,
              (io.grpc.stub.StreamObserver<org.apache.arrow.flight.impl.Flight.FlightData>) responseObserver);
          break;
        case METHODID_DO_ACTION:
          serviceImpl.doAction((org.apache.arrow.flight.impl.Flight.Action) request,
              (io.grpc.stub.StreamObserver<org.apache.arrow.flight.impl.Flight.Result>) responseObserver);
          break;
        case METHODID_LIST_ACTIONS:
          serviceImpl.listActions((org.apache.arrow.flight.impl.Flight.Empty) request,
              (io.grpc.stub.StreamObserver<org.apache.arrow.flight.impl.Flight.ActionType>) responseObserver);
          break;
        default:
          throw new AssertionError();
      }
    }

    @java.lang.Override
    @java.lang.SuppressWarnings("unchecked")
    public io.grpc.stub.StreamObserver<Req> invoke(
        io.grpc.stub.StreamObserver<Resp> responseObserver) {
      switch (methodId) {
        case METHODID_HANDSHAKE:
          return (io.grpc.stub.StreamObserver<Req>) serviceImpl.handshake(
              (io.grpc.stub.StreamObserver<org.apache.arrow.flight.impl.Flight.HandshakeResponse>) responseObserver);
        case METHODID_DO_PUT:
          return (io.grpc.stub.StreamObserver<Req>) serviceImpl.doPut(
              (io.grpc.stub.StreamObserver<org.apache.arrow.flight.impl.Flight.PutResult>) responseObserver);
        case METHODID_DO_EXCHANGE:
          return (io.grpc.stub.StreamObserver<Req>) serviceImpl.doExchange(
              (io.grpc.stub.StreamObserver<org.apache.arrow.flight.impl.Flight.FlightData>) responseObserver);
        default:
          throw new AssertionError();
      }
    }
  }

  public static final io.grpc.ServerServiceDefinition bindService(AsyncService service) {
    return io.grpc.ServerServiceDefinition.builder(getServiceDescriptor())
        .addMethod(
          getHandshakeMethod(),
          io.grpc.stub.ServerCalls.asyncBidiStreamingCall(
            new MethodHandlers<
              org.apache.arrow.flight.impl.Flight.HandshakeRequest,
              org.apache.arrow.flight.impl.Flight.HandshakeResponse>(
                service, METHODID_HANDSHAKE)))
        .addMethod(
          getListFlightsMethod(),
          io.grpc.stub.ServerCalls.asyncServerStreamingCall(
            new MethodHandlers<
              org.apache.arrow.flight.impl.Flight.Criteria,
              org.apache.arrow.flight.impl.Flight.FlightInfo>(
                service, METHODID_LIST_FLIGHTS)))
        .addMethod(
          getGetFlightInfoMethod(),
          io.grpc.stub.ServerCalls.asyncUnaryCall(
            new MethodHandlers<
              org.apache.arrow.flight.impl.Flight.FlightDescriptor,
              org.apache.arrow.flight.impl.Flight.FlightInfo>(
                service, METHODID_GET_FLIGHT_INFO)))
        .addMethod(
          getPollFlightInfoMethod(),
          io.grpc.stub.ServerCalls.asyncUnaryCall(
            new MethodHandlers<
              org.apache.arrow.flight.impl.Flight.FlightDescriptor,
              org.apache.arrow.flight.impl.Flight.PollInfo>(
                service, METHODID_POLL_FLIGHT_INFO)))
        .addMethod(
          getGetSchemaMethod(),
          io.grpc.stub.ServerCalls.asyncUnaryCall(
            new MethodHandlers<
              org.apache.arrow.flight.impl.Flight.FlightDescriptor,
              org.apache.arrow.flight.impl.Flight.SchemaResult>(
                service, METHODID_GET_SCHEMA)))
        .addMethod(
          getDoGetMethod(),
          io.grpc.stub.ServerCalls.asyncServerStreamingCall(
            new MethodHandlers<
              org.apache.arrow.flight.impl.Flight.Ticket,
              org.apache.arrow.flight.impl.Flight.FlightData>(
                service, METHODID_DO_GET)))
        .addMethod(
          getDoPutMethod(),
          io.grpc.stub.ServerCalls.asyncBidiStreamingCall(
            new MethodHandlers<
              org.apache.arrow.flight.impl.Flight.FlightData,
              org.apache.arrow.flight.impl.Flight.PutResult>(
                service, METHODID_DO_PUT)))
        .addMethod(
          getDoExchangeMethod(),
          io.grpc.stub.ServerCalls.asyncBidiStreamingCall(
            new MethodHandlers<
              org.apache.arrow.flight.impl.Flight.FlightData,
              org.apache.arrow.flight.impl.Flight.FlightData>(
                service, METHODID_DO_EXCHANGE)))
        .addMethod(
          getDoActionMethod(),
          io.grpc.stub.ServerCalls.asyncServerStreamingCall(
            new MethodHandlers<
              org.apache.arrow.flight.impl.Flight.Action,
              org.apache.arrow.flight.impl.Flight.Result>(
                service, METHODID_DO_ACTION)))
        .addMethod(
          getListActionsMethod(),
          io.grpc.stub.ServerCalls.asyncServerStreamingCall(
            new MethodHandlers<
              org.apache.arrow.flight.impl.Flight.Empty,
              org.apache.arrow.flight.impl.Flight.ActionType>(
                service, METHODID_LIST_ACTIONS)))
        .build();
  }

  private static abstract class FlightServiceBaseDescriptorSupplier
      implements io.grpc.protobuf.ProtoFileDescriptorSupplier, io.grpc.protobuf.ProtoServiceDescriptorSupplier {
    FlightServiceBaseDescriptorSupplier() {}

    @java.lang.Override
    public com.google.protobuf.Descriptors.FileDescriptor getFileDescriptor() {
      return org.apache.arrow.flight.impl.Flight.getDescriptor();
    }

    @java.lang.Override
    public com.google.protobuf.Descriptors.ServiceDescriptor getServiceDescriptor() {
      return getFileDescriptor().findServiceByName("FlightService");
    }
  }

  private static final class FlightServiceFileDescriptorSupplier
      extends FlightServiceBaseDescriptorSupplier {
    FlightServiceFileDescriptorSupplier() {}
  }

  private static final class FlightServiceMethodDescriptorSupplier
      extends FlightServiceBaseDescriptorSupplier
      implements io.grpc.protobuf.ProtoMethodDescriptorSupplier {
    private final java.lang.String methodName;

    FlightServiceMethodDescriptorSupplier(java.lang.String methodName) {
      this.methodName = methodName;
    }

    @java.lang.Override
    public com.google.protobuf.Descriptors.MethodDescriptor getMethodDescriptor() {
      return getServiceDescriptor().findMethodByName(methodName);
    }
  }

  private static volatile io.grpc.ServiceDescriptor serviceDescriptor;

  public static io.grpc.ServiceDescriptor getServiceDescriptor() {
    io.grpc.ServiceDescriptor result = serviceDescriptor;
    if (result == null) {
      synchronized (FlightServiceGrpc.class) {
        result = serviceDescriptor;
        if (result == null) {
          serviceDescriptor = result = io.grpc.ServiceDescriptor.newBuilder(SERVICE_NAME)
              .setSchemaDescriptor(new FlightServiceFileDescriptorSupplier())
              .addMethod(getHandshakeMethod())
              .addMethod(getListFlightsMethod())
              .addMethod(getGetFlightInfoMethod())
              .addMethod(getPollFlightInfoMethod())
              .addMethod(getGetSchemaMethod())
              .addMethod(getDoGetMethod())
              .addMethod(getDoPutMethod())
              .addMethod(getDoExchangeMethod())
              .addMethod(getDoActionMethod())
              .addMethod(getListActionsMethod())
              .build();
        }
      }
    }
    return result;
  }
}
